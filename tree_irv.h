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
