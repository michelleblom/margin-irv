#ifndef _SIM_IRV_H
#define _SIM_IRV_H

#include "model.h"

// Simulate IRV counting algorithm for given election, with given ballots,
// and candidates. The method will return the last round margin (x2), the
// total tally of each candidate (in votecounts), and the elimination
// sequence (in order_c). Set log to true to print round-by-round counts.
int SimIRV(const Ballots &ballots, const Doubles &votecounts, int &winner,
	Candidates &cands, const Config &config, Ints &order_c, bool log);

#endif
