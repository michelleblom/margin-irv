#ifndef _TREE_IRV_H
#define _TREE_IRV_H

#include "model.h"

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
	double timelimit, const char *logf, bool &timeout, int &dtcntr);

#endif
