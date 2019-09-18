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


#ifndef _MODEL_H
#define _MODEL_H

#include<vector>
#include<set>
#include<exception>
#include<boost/lexical_cast.hpp>
#include<boost/filesystem.hpp>
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string.hpp>
#include<map>

#ifndef _WIN32
#include<sys/time.h>
#endif

typedef std::vector<int> Ints;
typedef std::vector<double> Doubles;
typedef std::set<int> SInts;
typedef std::set<SInts> SInts2d;
typedef std::set<SInts2d> SInts3d;
typedef std::vector<Ints> Ints2d;
typedef std::vector<std::string> Strings;

struct mytimespec
{
	double seconds;
};

void GetTime(struct mytimespec* t);


struct Config
{
	int ncandidates;
	double totalvotes;

	bool tightbounds;
	bool compbounds;
	bool optlog;

	std::map<int,int> id2index;
	Strings elect_only;

	Config() : ncandidates(0), totalvotes(0), tightbounds(false),
		compbounds(false), optlog(false) {}
};

class STVException
{
	private:
		const std::string message;

	public:
		STVException(const STVException &me) : message(me.what()){}
		STVException(const std::string &str) : message(str) {}
		const std::string& what() const { return message; }
};


struct Candidate
{
	int id;
	int index;
	double sum_votes;

	Ints ballots;

	std::string party;

	// For simulation
	int sim_votes;
	Ints sim_ballots;

	int standing;

	Candidate() : id(0), index(0), sum_votes(0), party("IND"),
		sim_votes(0), standing(1) {}
};

typedef std::vector<Candidate> Candidates;

struct Ballot
{
	int tag;
	double votes; // Number of votes present with this signature
	Ints prefs;
};

typedef std::vector<Ballot> Ballots;

template <typename T>
T ToType(const std::string &s);

typedef std::map<std::vector<int>,int> I2Map;

struct Node{
	// Score (or LP evaluation)
	double dist;

	// Elimination sequence (partial or total)
	Ints order_c;

	// Set of candidates *not* in elimination sequence
	SInts remcand;

	// The following three attributes used when solving LP
	// for this node.
	Ballots rev_ballots;
	I2Map ballotmap;
	Ints bid2newid;

	Node(int ncand, int nballots) {
		bid2newid.resize(nballots, -1);
		dist = -1;
	}

	void Reset(){
		dist = -1;

		rev_ballots.clear();
		ballotmap.clear();
		remcand.clear();

		for(int i = 0; i < bid2newid.size(); ++i){
			bid2newid[i] = -1;
		}
	}

	void ClearEqClassData(){
		rev_ballots.clear();
		ballotmap.clear();
	}
};


bool ReadBallots(const char *path, Ballots &ballots,
	Candidates &candidates, Config &config);

#endif
