#include<set>
#include<vector>
#include<list>
#include<iostream>
#include<fstream>
#include<cmath>
#include<sstream>

#include "tree_irv.h"
#include "irv_distance.h"

using namespace std;

typedef list<Node> Fringe;
typedef vector<Node> Nodes;

// Node 'n' will be inserted into the fringe so that
// the fringe remains sorted in order of n.dist. 
void InsertIntoFringe(const Node &n, Fringe &fringe){
	Fringe::iterator it = fringe.begin();
	for( ; it != fringe.end(); ++it){
		if(n.dist < it->dist || (n.dist == it->dist &&
			n.order_c.size() > it->order_c.size())){
			break;
		}
	}
	
	fringe.insert(it, n);
}


void PrintNode(const Node &n, ostream &log){
	for(int i = 0; i < n.order_c.size(); ++i){
		log << n.order_c[i] << " ";
	}

	log << " with distance " << n.dist << " ";
}

// Print first and last nodes of the search frontier.
void PrintFringe(const Fringe &fringe, ostream &log){
	log << "--------------------------------" << endl;
	log << "Current state of priority queue: " << endl;
	log << "    Node ";
	PrintNode(fringe.front(), log);
	log << endl;
	log << "    .... " << endl;
	log << "    Node ";
	PrintNode(fringe.back(), log);
	log << endl;

	log << "--------------------------------" << endl;
}

// Given a node 'n', we expand the node by creating a child for every
// candidate 'c' that is not in the node's current elimination order. In
// each of these new nodes, 'c' is appended to the front of the 
// elimination sequence. 
void GetChildren(int ncand, int nballots, const Node &n, Nodes &children){
	for(SInts::const_iterator it = n.remcand.begin(); 
		it != n.remcand.end(); ++it){
		Node newn(ncand, nballots);
		newn.order_c = n.order_c;
		newn.order_c.insert(newn.order_c.begin(), *it);
		newn.remcand = n.remcand;
		newn.remcand.erase(*it);
		newn.dist = n.dist; 

		children.push_back(newn);
	}
}

// Remove all nodes whose current scores/distance values are greater than
// or equal to the current upper bound (ubound).
void PruneFringe(Fringe &fringe, double ubound, ostream &log, bool dolog){
	for(Fringe::iterator it = fringe.begin(); it != fringe.end(); ){
		if(it->dist >= ubound){
			if(dolog){
				log << "Pruning node ";
				PrintNode(*it, log);
				log << endl;
			}

			if(fringe.size() == 1){
				fringe.clear();
				return;
			}

			fringe.erase(it++);
		}
		else{
			++it;		
		}
	}
}


// Implements branch and bound search given:
//   INPUT
//   ballots:    vector of ballot signatures in the original election
//   cands:      vector of Candidate structures 
//   config:     basic parameters
//   altwinners: indices of candidates that we want to consider as
//               'alternate winners'
//   upperbound: starting upper bound on margin.
//   timelimit:  timelimit (in seconds) after which search terminates.
//   logf:       file for logging (NULL if not logging)
//
//   OUTPUT
//   timeout:    True if search times out, false otherwise
//   dtcntr:     Number of 'distance to' LPs solved
//   
//   RETURNS
//   margin:     Margin for election (or lower bound on margin if
//               search times out).    
double RunTreeIRV(const Ballots &ballots, const Candidates &cands,
	const Config &config, const Ints &altwinners, int upperbound, 
	double timelimit, const char *logf, bool &timeout, int &dtcntr)
{
	try{
		mytimespec start;
		GetTime(&start);

		Fringe fringe;

		ofstream log;
		bool dolog = false;
		if(logf != NULL){
			log.open(logf);
			dolog = true;
		}
	
		// BUILD FRINGE: we are interested in one of the candidates in
		// 'altwinners' winning the election.
		for(int i = 0; i < altwinners.size(); ++i){
			Node newn(cands.size(), ballots.size());
			newn.dist = 0;
			newn.order_c.push_back(altwinners[i]);
			for(int j = 0; j < cands.size(); ++j){
				if(j != altwinners[i]){
					newn.remcand.insert(j);
				}
			}

			if(config.compbounds){
				// Evaluate lower bound on margin for node
				ApplyScoringRules(ballots, cands, config, newn);
				if(dolog){
					log << "Score for ";
					PrintNode(newn, log);
					log << endl;
				}
			}

			// Run distance to get more precise lower bound
			if(newn.dist >= 0 && newn.dist < upperbound){
				InsertIntoFringe(newn, fringe);
			}
		}

		Ints best_order_c;

		double curr_ubound = upperbound;

		timeout = false;
		while(fringe.size() > 0){
			if(dolog){
				PrintFringe(fringe, log);
				log << "CURRENT UPPER BOUND = " << curr_ubound << endl;
			}

			double blower = curr_ubound;
			for(Fringe::const_iterator it = fringe.begin();
				it != fringe.end(); ++it){
				blower = min(blower, it->dist);
			}

			if(dolog){
				log << "BEST LOWER BOUND = " << blower << endl;
			}

			mytimespec tnow;
			GetTime(&tnow);

			// Expand first node in fringe: Get/evaluate children 
			Node expand = *(fringe.begin());
			fringe.erase(fringe.begin());

			if(dolog){
				log << "Expanding ";
				PrintNode(expand, log);
				log << endl;
			}

			Nodes children;
			GetChildren(config.ncandidates, ballots.size(), expand, children);

			double tleft = -1;
			int nchildrenadded = 0;
			for(int i = 0; i < children.size(); ++i){
				if(timelimit != -1){
					mytimespec tnow;
					GetTime(&tnow);
				
					tleft = timelimit - (tnow.seconds-start.seconds);
					if(tleft <= 0){
						timeout = true;
						break;
					}
				}

				Node &child = children[i];
				if(config.compbounds){
					// Evaluate lower bound on margin for node
					ApplyScoringRules(ballots, cands, config, child);
					if(dolog){
						log << "Score for child ";
						PrintNode(child, log);
						log << endl;
					}
				}

				if(child.dist >= curr_ubound){
					if(dolog){
						log << "    skipping child" << endl;
					}
					continue;
				}

				child.dist = distance(ballots, cands, config, child, 
					curr_ubound, tleft, log, dolog, timeout);
				++dtcntr;

				if(dolog){
					log << "    DT value: " << child.dist << endl;
				}

				if(timeout){
					break;
				} 

				if(child.dist < 0)
					continue;

				if(child.dist >= 0 && child.dist < curr_ubound){
					if(dolog){
						log << "Adding node to fringe: ";
						PrintNode(child, log);
						log << endl;
					}
					InsertIntoFringe(child, fringe);
					nchildrenadded += 1;
				}

				if(child.remcand.empty() && child.dist < curr_ubound){
					curr_ubound = child.dist;

					best_order_c = child.order_c;

					// Update current upper bound if a leaf found.
					PruneFringe(fringe, curr_ubound, log, dolog);
				}
			}

			if(timeout){
				break;
			}
		}

		mytimespec tnow;
		GetTime(&tnow);
		if(dolog){
			log << "TOTAL TIME USED SO FAR: " << tnow.seconds -
				start.seconds << endl;
		}

		if(dolog && !timeout){
			if(!best_order_c.empty()){
				log << "====================================" <<endl;
				log << "Minimal manipulation: " << curr_ubound << endl;
				log << "Manipulated order: ";
				for(int i = 0; i < best_order_c.size(); ++i){
					log << cands[best_order_c[i]].id << " ";
				}
				log << endl;
			}
			else{
				log << "All nodes pruned " << endl;
			}

			log << "Distance calls: " << dtcntr << endl;
			log << "Margin: " << curr_ubound << endl;
			log << "====================================" <<endl;
			log.close();
		}

		double blower = curr_ubound;
		for(Fringe::const_iterator it = fringe.begin();
			it != fringe.end(); ++it){
			blower = min(blower, it->dist);
		}

		if(dolog && timeout){
			log << "Timeout: bounds on margin are [" << 
				blower << "," << curr_ubound << "]" << endl; 		
		}

		if(dolog){
			log.close();
		}

		if(timeout){
			return blower;
		}
		else{
			return curr_ubound;
		}
	}
	catch(exception &e)
	{
		cout << "Exception raised in RunTreeIRV" << endl;
		cout << e.what() << endl;
		return -1;
	}
	catch(STVException &e)
	{
		cout << "Exception raised in RunTreeIRV" << endl;
		cout << e.what() << endl;
		return -1;
	}	
	catch(...)
	{
		cout << "Exception raised in RunTreeIRV" << endl;
		cout << "Unexpected error." << endl;
		return -1;
	}
}



