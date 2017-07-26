#ifndef _STV_DISTANCE_H
#define _STV_DISTANCE_H

#include "model.h"

// Apply basic or tight scoring rules to evaluate a partial (or total)
// elimination sequence prior to solving an LP for it.
//
// INPUT
// ballots:  Original ballot signatures in election
// cand:     Set of candidates in election (Candidate data structures)
// config:   Basic parameters
// node:     Node to evaluate
//
// OUTPUT:
// node.dist will be set to the score calculated (if higher than its
// current evaluation)
void ApplyScoringRules(const Ballots &ballots, const Candidates &cand,
	const Config &config, Node &node);

// Solve LP to get a score for a partial node (lower bound on the 
// number of vote manipulations required to realise its elimination
// sequence) or an exact margin for a node with a complete sequence.
//
// INPUT
// ballots:    Original ballot signatures in election
// cand:       Set of candidates in election (Candidate data structures)
// config:     Basic parameters
// node:       Node to evaluate
// upperbound: Current upper bound on margin
// tleft:      Remaining timelimit for algorithm (-1 if no timelimit)
// log:        Stream to print logging details to.
// dolog:      Boolean to indicate whether to print logging information.
//
// OUTPUT:
// Minimum manipulations required to realise node elimination sequence. This
// will be a lower bound if it is a partial sequence.
//
// timeout:    Will be set to true if the time limit ran out, and consequently
//             the value returned will not be the minimum.
double distance(const Ballots &ballots, const Candidates &cand, 
	const Config &config, Node &node, double upperbound,
	double tleft,  std::ofstream &log, bool dolog, bool &timeout);


#endif
