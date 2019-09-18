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


#include<fstream>
#include<algorithm>
#include<iostream>
#include<sstream>
#include<time.h>
#include "model.h"

using namespace std;
typedef boost::char_separator<char> boostcharsep;

void GetTime(struct mytimespec *t)
{
	#ifndef _WIN32
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t->seconds = tv.tv_sec + (tv.tv_usec/1E6);
	#else
	t->seconds = clock()/CLOCKS_PER_SEC;
	#endif
}

template <typename T>
T ToType(const std::string &s) 
{ 
	try
	{
		return boost::lexical_cast<T>(s); 
	}
	catch(...)
	{
		throw STVException("Lexical cast problem");
	}
}

void Split(const string &line, const boostcharsep &sep, vector<string> &r)
{
	try
	{
		boost::tokenizer<boostcharsep> tokens(line, sep);
		r.assign(tokens.begin(), tokens.end());

		for(int i = 0; i < r.size(); ++i)
		{
			boost::algorithm::trim(r[i]);
		}
	}
	catch(exception &e)
	{
		stringstream ss;
		ss << e.what();
		throw STVException(ss.str());
	}
	catch(STVException &e)
	{
		throw e;
	}
	catch(...)
	{
		throw STVException("Error in Split Function.");
	}
}


// Assumed input format:
// (first_id, second_id, third_id, ...) : #appears
bool ReadBallots(const char *path, Ballots &ballots, Candidates &candidates,
	Config &config)
{
	try
	{
		ifstream infile(path);

		// First line is list of candidates.
		cout << "Reading Candidates" << endl;
		boostcharsep spcom(",");
		string line;
		getline(infile, line);

		vector<string> columns;
		Split(line, spcom, columns);

		config.ncandidates = columns.size();
		for(int i = 0; i < columns.size(); ++i)
		{
			int id = ToType<int>(columns[i]);
			Candidate c;
			c.index = i;
			c.id = id;

			candidates.push_back(c);

			config.id2index.insert(pair<int,int>(id,i));
		}

		// Read party affiliations
		getline(infile, line);
		columns.clear();
		Split(line, spcom, columns);
		for(int i = 0; i < columns.size(); ++i)
		{
			candidates[i].party = columns[i];
		}

		// Skip next line (separator)
		getline(infile, line);

		cout << "Reading Ballots" << endl;
		boostcharsep sp(",():");
		
		int cntr = ballots.size();
		while(getline(infile, line))
		{
			vector<string> columns;
			Split(line, sp, columns);

			Ballot b;
			b.tag = cntr;
			b.votes = ToType<double>(columns.back());

			for(int i = 0; i < columns.size()-1; ++i)
			{
				if(columns[i] == "") continue;
				int ccode = ToType<int>(columns[i]);
				int index = config.id2index.find(ccode)->second;
					
				if(find(b.prefs.begin(),
					b.prefs.end(), index) != b.prefs.end())
				{
					continue;
				}

				b.prefs.push_back(index);
			}

			if(b.prefs.empty()) continue;

			Candidate &cand = candidates[b.prefs.front()];
			bool found_ballot = false;
			int bid = -1;
			for(int j = 0; j < cand.ballots.size(); ++j){
				const Ballot &bf = ballots[cand.ballots[j]];
				if(bf.prefs == b.prefs){
					bid = bf.tag;
					found_ballot = true;
					break;
				}
			}
			if(found_ballot){
				ballots[bid].votes += b.votes;
				cand.sum_votes += b.votes;
				config.totalvotes += b.votes;
				continue;
			}
			else{
				cand.sum_votes += b.votes;
				cand.ballots.push_back(b.tag);

				config.totalvotes += b.votes;
	
				ballots.push_back(b);
				++cntr;
			}
		}

		cout << "Finished reading ballots" << endl;
		infile.close();
	}
	catch(exception &e)
	{
		throw e;
	}
	catch(STVException &e)
	{
		throw e;
	}
	catch(...)
	{
		cout << "Unexpected error reading in ballots." << endl;
		return false;
	}

	return true;
}
