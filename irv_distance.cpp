#include<vector>
#include<map>
#include<string>
#include<assert.h>
#include<iostream>
#include<fstream>
#include<stdio.h>
#include "math.h"

#include "cplex_utils.h"
#include "irv_distance.h"
#include "sim_irv.h"


using namespace std;

void ApplyScoringRules(const Ballots &ballots, const Candidates &cand,
	const Config &config, Node &node){

	try{
		// Compute lower bounding rules for partial order
		int lbound = max(0.0, node.dist);

		if(config.tightbounds){
			// Tighter lower bound
			int lb2 = 0;

			Ints alive(config.ncandidates, 0);
			for(int j = 0; j < node.order_c.size(); ++j){
				alive[node.order_c[j]] = 1;
			}
		
			for(SInts::const_iterator it = node.remcand.begin();
				it != node.remcand.end(); ++it){
				const Candidate &e = cand[*it];
				const Candidate &c = cand[node.order_c[0]];
				int vcntr = 0;

				for(Ballots::const_iterator bt = ballots.begin();
					bt != ballots.end(); ++bt){
					for(Ints::const_iterator jt = bt->prefs.begin();
						jt != bt->prefs.end(); ++jt){
						if(*jt == c.index){
							vcntr += bt->votes;
							break;
						}
						else if(*jt == e.index){
							break;
						}
						else if(alive[*jt] == 1){
							break;
						}
					}
				}		

				int diff = max(0.0, ceil((e.sum_votes - vcntr)/2.0));
				lb2 = max(diff, lb2);
			}

			lbound = max(lbound, lb2);
		}
		else{
			int lb1 = 0;
			for(SInts::const_iterator it = node.remcand.begin();
				it != node.remcand.end(); ++it){
				const Candidate &e = cand[*it];
				// 'e' must be eliminated before candidates
				// currently in node.order_c.

				// We need to make sure the primary vote of 'e' is less
				// than or equal to the number of votes in which each
				// candidate 'c' in node.order_c is ranked higher than 'e'
				// (or 'c' appears and 'e' does not). We will have
				// computed the lower bound as each candidate was added
				// to the tree, so we only need to focus on the first 
				// candidate in the partial order.
				const Candidate &c = cand[node.order_c[0]];
				int vcntr = 0;

				for(Ballots::const_iterator bt = ballots.begin();
					bt != ballots.end(); ++bt){
					for(Ints::const_iterator jt = bt->prefs.begin();
						jt != bt->prefs.end(); ++jt){
						if(*jt == c.index){
							vcntr += bt->votes;
							break;
						}
						else if(*jt == e.index){
							break;
						}
					}
				}		

				int diff = max(0.0, ceil((e.sum_votes - vcntr)/2.0));
				lb1 = max(diff, lb1);
			}

			lbound = max(lb1, lbound);
		}

		node.dist = lbound;
	}
	catch(exception e){
		throw e;
	}
	catch(...){
		throw STVException("Unexpected error in ApplyScoringRules");
	}
}



int next_mod(vector<int> &mask, int n){
	int i;
	for(i = 0; i < n && mask[i]; ++i){
		mask[i] = 0;
	}

	if(i < n){
		mask[i] = 1;
		return 1;
	}
	return 0;
}


void CreateEquivalenceClasses(const Ballots &ballots, 
	const Candidates &cand, const Config &config, Node &node,
	const Ints &position){
	try{
		const int ncand = node.order_c.size();

		Ints mask(config.ncandidates, 0);

		int cntr = 0;
		while(next_mod(mask, ncand)){
			int j = -1;
			for(int i = 0; i < ncand; ++i){
				if(mask[i]){
					j = i;
					break;
				}
			}

			if(j < 0) continue;
	
			vector<int> key(ncand, 0);
			for(int i = 0; i < ncand; ++i){
				if(mask[i])
					key[i] = 1;
			}

			Ballot b;
			b.tag = cntr++;
			b.votes = 0;

			b.prefs.push_back(node.order_c[j]);
			for(int i = j+1; i < ncand; ++i){
				if(mask[i]){
					b.prefs.push_back(node.order_c[i]);
				}
			}

			node.rev_ballots.push_back(b);
			node.ballotmap.insert(pair<vector<int>,int>(key, b.tag));
		}

		for(int b = 0; b < ballots.size(); ++b){
			const Ballot &bt = ballots[b];
			Ints key;
			key.resize(ncand, 0);

			int maxj = 0;
			for(int i = 0; i < bt.prefs.size(); ++i){
				int j = position[bt.prefs[i]];

				if(j == -1)
					continue;

				if(j >= maxj){
					key[j] = 1;
					maxj = j;

					if(j == ncand - 1){
						break;
					}
				}
			}
	
			if(node.ballotmap.find(key) == node.ballotmap.end()){
				node.bid2newid[b] = -1;
				continue;
			}
		
			int id = node.ballotmap.find(key)->second;
			node.rev_ballots[id].votes += bt.votes;
			node.bid2newid[b] = id;
		}
	}
	catch(STVException &e)
	{
		throw e;
	}
	catch(...)
	{
		throw STVException("Unexpected error in creating eq classes.");
	}
}


double distance(const Ballots &ballots, const Candidates &cand, 
	const Config &config, Node &node, double upperbound,
	double tleft, ofstream &log, bool dolog, bool &timeout){

	double dist = -1;

	try{
		const int ncand = node.order_c.size();	
		Ints position(config.ncandidates, -1);

		for(int i = 0; i < node.order_c.size(); ++i){
			position[node.order_c[i]] = i;
		}

		CreateEquivalenceClasses(ballots, cand, config, node, position);

		IloEnv env;
		IloModel cmodel(env);

		// Assume ballots contains all possible rankings, even 
		// if the number of times that ranking was voted for is '0'
		const int sigs = node.rev_ballots.size();

		IloNumVarArray ps(env, sigs);
		IloNumVarArray ms(env, sigs);
		IloNumVarArray ys(env, sigs);

		char varname[500];

		IloExpr balance(env);

		double lb = max(0.0, node.dist);
		double ub = max(lb, upperbound);

		IloExpr obj(env);

		for(int i = 0; i < sigs; ++i){
			const double ns = node.rev_ballots[i].votes;

			// p_s variable: number of ballots modified so that their
			// new signature is 's' 
			sprintf(varname, "vps_%d", i);
			if(ncand == config.ncandidates){
				ps[i] = IloNumVar(env, 0, ub, ILOINT, varname);
			}
			else{
				ps[i] = IloNumVar(env, 0, ub, ILOFLOAT, varname);
			}

			// m_s variable: number of ballots whose signature in the
			// original profile is 's', but are modified to something 
			// other than 's' in the new profile
			sprintf(varname, "vms_%d", i);
			if(ncand == config.ncandidates){
		    	ms[i] = IloNumVar(env, 0, min(ns, ub), ILOINT, varname);
			}
			else{
		    	ms[i] = IloNumVar(env, 0, min(ns, ub), ILOFLOAT, varname);
			}

			// y_s variable: total number of ballots with signature 's'
			// in the new election profile
			sprintf(varname, "vys_%d", i);

			ys[i] = IloNumVar(env, 0, min(ns+ub,config.totalvotes), 
			    ILOFLOAT, varname);

			// n_s = total number of ballots with signature 's' in
			// the original profile.
			// constraint: n_s + p_s - m_s = y_s
			//     rewrite: n_s = y_s + m_s - p_s
			cmodel.add(ns == ys[i] + ms[i] - ps[i]);
			balance += (ps[i] - ms[i]);

			obj += ps[i];
		}

		cmodel.add(obj >= lb);
		cmodel.add(obj <= ub);
		cmodel.add(IloMinimize(env, obj));

		cmodel.add(balance == 0);

		// Constraints to ensure elimination order proceeds as stated
		for(int round = 0; round < ncand-1; ++round){
			// The candidate 'ec' eliminated in this round must have less than
			// (or equal to) votes than everyone still remaining.
			IloExpr yr(env); // Votes in tally of 'e'

			const int ec = node.order_c[round];

			Ints2d poss_tally(config.ncandidates);

			for(int i = 0; i < sigs; ++i){
				// will this ballot signature count toward 'ec'
				const Ballot &bt = node.rev_ballots[i];
				for(int j = 0; j < bt.prefs.size(); ++j){
					if(bt.prefs[j] == ec){
						yr += ys[i];
						break;
					}
					else if(position[bt.prefs[j]] > round){
						// Ballot with id 'i' will be in bt.prefs[j]'s tally
						poss_tally[bt.prefs[j]].push_back(i);
						break;
					} 
				}
			} 

			for(int j = round+1; j < ncand; ++j){
				// How many votes does the candidate eliminated in position
				// 'j' have right now?
				const int cc = node.order_c[j];

				IloExpr yr_cc(env);
				const Ints &intally = poss_tally[cc];

				for(int k = 0; k < intally.size(); ++k){
					yr_cc += ys[intally[k]];
				}

				if(intally.size() > 0){
					cmodel.add(yr <= yr_cc);
				}
			}
		}
		

		IloCplex cplex(cmodel);
		if(dolog && config.optlog){
			cplex.setOut(log);
		}
		else{
			cplex.setOut(env.getNullStream());
		}

		cplex.setWarning(env.getNullStream());
		if(tleft >= 0){
        	cplex.setParam(IloCplex::TiLim, tleft);
		}

		node.ClearEqClassData();
		bool result = cplex.solve();

		if(cplex.getCplexStatus() == IloCplex::Infeasible){
			dist = -1;
		}
		else if(result){
  		    dist = cplex.getObjValue();
		} 
		else{
			timeout = true;
			dist = cplex.getObjValue();
		}

		cplex.end();
		cmodel.end();
		env.end();

	}
	catch(IloCplex::Exception e){
		stringstream ss;
		ss << "CPLEX error in STV distance calc: " << e.getMessage() << endl;
		throw STVException(ss.str());
	}
	catch(STVException &e)
	{
		throw e;
	}
	catch(...)
	{
		throw STVException("Unexpected error in STV distance calculation.");
	}


	return ceil(dist);
}


