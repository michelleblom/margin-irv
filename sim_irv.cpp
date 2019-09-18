/*
    Copyright (C) 2016-2019  Michelle Blom

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "sim_irv.h"
#include<iostream>

using namespace std;

int NextCandidate(const Ballot &b, int id, const Candidates &cand);

int SimIRV(const Ballots &ballots, const Doubles &votecounts, int &winner, 
	Candidates &cand,const Config &config,Ints &order_c,bool log)
{
	int last_round_margin = 0;
	try	{
		if(log) cout << "First preference tallies: " << endl;

		int lowest_idx = -1;
		int lowest_votes = 0;

		// First compute initial tallies
		for(int c = 0; c < cand.size(); ++c){
			Candidate &ct = cand[c];
			ct.sim_votes = 0;
			ct.standing = 1;

			for(Ints::const_iterator li = ct.ballots.begin();
				li != ct.ballots.end(); ++li){
				ct.sim_votes += votecounts[*li];
				ct.sim_ballots.push_back(*li);
			}

			if(lowest_idx == -1 || ct.sim_votes < lowest_votes){
				lowest_idx = c;
				lowest_votes = ct.sim_votes;
			}

			if(log){
				cout << "  Candidate " << ct.id << " " << ct.sim_votes << endl;
			}
		}

		if(log)
			cout << endl;

		// Eliminate candidate with smallest tally, until there is
		// only 1 candidate left -- the winner.
		for(int r = 0; r < cand.size() - 1; ++r){
			// Eliminate candidate with the least votes
			int lowest_votes = -1;
			int lowest_idx = -1;

			for(int c = 0; c < cand.size(); ++c){
				const Candidate &ct = cand[c];

				if(ct.standing == 0){
					continue;
				}

				if(lowest_idx == -1 || ct.sim_votes < lowest_votes){
					lowest_idx = c;
					lowest_votes = ct.sim_votes;
				}
			}

			Candidate &e = cand[lowest_idx];
			order_c.push_back(lowest_idx);

			if(log){
				cout << "Round " << r << ", Candidate " << 
					e.id << " eliminated." << endl;
			}

			e.standing = 0;

			if(r == cand.size() - 2){
				// one candidate remains -- the winner
				last_round_margin = lowest_votes;
				break;
			}

			Ints2d totransfer(cand.size());

			for(Ints::iterator it = e.sim_ballots.begin();
				it != e.sim_ballots.end(); ++it){
				int next = NextCandidate(ballots[*it],lowest_idx,cand);

				if(next >= 0){
					totransfer[next].push_back(*it);
				}
			}

			e.sim_votes = 0;

			for(int i = 0; i < totransfer.size(); ++i){
				const Ints &list = totransfer[i];
				if(list.empty()) continue;
			
				Candidate &ct = cand[i];
				int total = 0;
				for(int j = 0; j < list.size(); ++j){
					total += votecounts[list[j]];
					ct.sim_ballots.push_back(list[j]);
				}

				ct.sim_votes += total;
				if(log) {
					cout << total << " votes distributed from " << e.id <<
						" to " << ct.id << endl;
				}
			}
			if(log){
				for(int c = 0; c < cand.size(); ++c){
					Candidate &ct = cand[c];
					if(ct.standing == 1){
						cout << "  Candidate " << ct.id << " " << 
							ct.sim_votes << endl;
					}
				}
				cout << endl;
			}
		}
		

		for(int c = 0; c < cand.size(); ++c){
			const Candidate &ct = cand[c];
			if(ct.standing == 1){
				order_c.push_back(c);
				last_round_margin = ct.sim_votes - last_round_margin;
				winner = c;
				if(log){
					cout << "Candidate " << ct.id << " elected." << endl;
				}
				break;
			}
		}

		if(log){
			cout << "Elimination order: ";
			for(int i = 0; i < order_c.size(); ++i){
				cout << order_c[i] << " ";
			}
			cout << endl;
		}
	}
	catch(STVException &e)
	{
		throw e;
	}
	catch(...)
	{
		throw STVException("Unexpected error in IRV simulation.");
	}

	if(log) cout << "Simulation complete" << endl << endl;
	return last_round_margin;
}

// Determines the next eligible candidate in b.prefs, after 'index'.
int NextCandidate(const Ballot &b, int index, const Candidates &cand)
{
	try{
		int idx = find(b.prefs.begin(), b.prefs.end(), index)-b.prefs.begin();
	
		for(int i = idx+1; i < b.prefs.size(); ++i){
			if(cand[b.prefs[i]].standing == 0) 
				continue;

			return b.prefs[i];
		}

		return -1;
	}
	catch(exception &e){
		throw STVException(string(e.what()));
	}
	catch(...){
		throw STVException("Unexpected error in NextCandidate.");
	}

	return -1;
}

