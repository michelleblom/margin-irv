// Code written by Michelle Blom, July 2017
// Please send any queries/bug notifications to michelle.blom@unimelb.edu.au

#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>
#include<algorithm>

#include "model.h"
#include "sim_irv.h"
#include "math.h"

#include "tree_irv.h"

using namespace std;

// USAGE: marginstv -ballots [ballot file] [-score] [-tight] [-simlog] [-optlog]
//            [-tlimit value] [-logfile logfilename] [-electonly N parties]
//
// -score:     Apply basic scoring rules to prune search
// -tight:     Apply tighter scoring rules to prune search (supercedes score)
// -simlog:    Print details of IRV simulation of original election to stdout
// -optlog:    Print CPLEX solving information to logfile
// -tlimit:    Timelimit (in s) after which branch and bound will terminate
// -logfile:   File to direct search log
// -electonly: [optional] 
//              N (number of alternative winners we want to consider)
//              Party1 Party2 ... PartyN
//
//              Example: -electonly 2 LIB LAB
//
// NOTE: This code implements Blom et. al.'s modification of Magrino et. al.'s 
// margin computation algorithm (by adding lower bounding rules to prune
// the search space). When run on the IRV instances considered by Blom et. al.,
// slightly different values for the number of LPs solved will be obtained 
// (than those reported in the paper). This (updated) c++ implementation 
// creates and adds children of an expanded node in a slightly different order 
// than the old c implementation -- this means that nodes with the same 
// 'score' will be situated in the 'search frontier' or queue in a slightly 
// different order. However, the number of LP solves required when using the
// basic lower bounding rule vs the tighest lower bounding rule vs no
// lower bounding rules (the setting considered by Magrino et. al.) follows
// the same pattern reported in Blom et. al.'s paper (number of LPs required
// with no bounding rules >> LPs required with the basic bounding rules used
// > LPs required with the tightest bounding rules used).
int main(int argc, const char * argv[]) 
{
	try{
		Candidates candidates;
		Ballots ballots; 
		Config config;

		const char *logf = NULL;
		bool simlog = false;
		double timelimit = -1;

		for(int i = 1; i < argc; ++i){
			if(strcmp(argv[i], "-ballots") == 0 && i < argc-1){
				if(!ReadBallots(argv[i+1], ballots, candidates, config)){
					cout << "Ballot read error. Exiting." << endl;
					return 1;
				}
				++i;
			}
			else if(strcmp(argv[i], "-score") == 0){
				config.compbounds = true;
			}
			else if(strcmp(argv[i], "-simlog") == 0){
				simlog = true;
			}
			else if(strcmp(argv[i], "-optlog") == 0){
				config.optlog = true;
			}
			else if(strcmp(argv[i], "-tight") == 0){
				config.tightbounds = true;
				config.compbounds = true;
			}
			else if(strcmp(argv[i], "-tlimit")== 0 && i < argc-1){
				timelimit = atoi(argv[i+1]);
				++i;
			}
			else if(strcmp(argv[i], "-logfile")== 0 && i < argc-1){
				logf = argv[i+1];
				++i;
			}
			else if(strcmp(argv[i], "-electonly") == 0){
				int n_in_list = atoi(argv[i+1]);
				for(int j = 0; j < n_in_list; ++j){
					config.elect_only.push_back(string(argv[i+2+j]));
				}
				i += 1 + n_in_list;
			}
		}

		double upperbound = config.totalvotes;

		mytimespec start;
		GetTime(&start);

		Ints order_c;
	
		Doubles votecounts(ballots.size(), 0);
		for(int i = 0; i < ballots.size(); ++i){
			votecounts[i] = ballots[i].votes;
		}

		// Simulate IRV election to determine winner, and last round margin
		int winner = -1;
		int lrmargin = SimIRV(ballots, votecounts, winner,
			candidates, config, order_c, simlog);

		// Compile list of alternative winners we wish to consider: will
		// depend on whether the -electonly flag has been specified.
		Ints altwinners;
		if(config.elect_only.empty()){
			// No elect only flag: consider all candidates who did not win
			// Starting upper bound is the last round margin
			upperbound = ceil(lrmargin/2.0);
			for(int i = 0; i < config.ncandidates; ++i){
				if(i == winner)
					continue;
				
				altwinners.push_back(i);
			}
		}
		else{
			// Consider only those candidates with the specified party
			// affliations (upper bound is total number of votes).
			for(int i = 0; i < config.ncandidates; ++i){
				const Candidate &c = candidates[i];
				if(find(config.elect_only.begin(), 
					config.elect_only.end(), c.party) !=
					config.elect_only.end()){
					altwinners.push_back(i);
				}
			}
		}

		const Candidate &cw = candidates[winner];
		if(find(config.elect_only.begin(), 
			config.elect_only.end(), cw.party) !=
			config.elect_only.end()){

			// Original winner in 'elect only' set
			cout << "Margin: 0" << endl;
			return 0;
		}

		// Run branch and bound
		bool timeout = false;
		int dtcntr = 0;
		double r = RunTreeIRV(ballots, candidates, config, altwinners,
			upperbound, timelimit, logf, timeout, dtcntr);

		if(r == -1){
			// Exception was raised.
			return 1;
		}

		mytimespec tend;
		GetTime(&tend);

		if(config.elect_only.empty()){
			cout << "LRM:        " << ceil(lrmargin/2.0)<<endl;
		}

		if(timeout){
			cout << "Margin LB:  " << r << endl;
		}
		else{
			cout << "Margin:     " << r << endl;
		}
		cout << "LPs solved: " << dtcntr << endl;
		cout << "Total time: " << tend.seconds - start.seconds << endl;
	}
	catch(exception &e)
	{
		cout << e.what() << endl;
		cout << "Exiting." << endl;
		return 1;
	}
	catch(STVException &e)
	{
		cout << e.what() << endl;
		cout << "Exiting." << endl;
		return 1;
	}	
	catch(...)
	{
		cout << "Unexpected error. Exiting." << endl;
		return 1;
	}

	return 0;
}




