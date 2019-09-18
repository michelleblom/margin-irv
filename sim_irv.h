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
