/*
 * SequenceSet.cpp
 *
 *  Created on: Sep 6, 2015
 *      Author: CarstenK
 *		 Email: CarstenK[@]posteo.de
 *	 Copyright: 2015
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Carsten Kemena
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SequenceSet.hpp"

namespace fs = boost::filesystem;

using namespace std;

namespace BBL
{

SequenceSet::SequenceSet()
{
	// TODO Auto-generated constructor stub

}

SequenceSet::~SequenceSet()
{
	// TODO Auto-generated destructor stub
}


SequenceSet::SequenceSet(boost::filesystem::path &path)
{
	read(path);
}


void
SequenceSet::readFasta_(AlgorithmPack::Input &inS, const std::string &re, short place, bool keep)
{
	regex m(re);
	std::string line;
	Sequence *p = nullptr;
	//auto &indexS = inS.get();
	while (getline(inS, line))
	{
		size_t nameEnd;
		if (line.empty())
			continue;
		if (line[0] == '>')
		{
			size_t start, end;
			if ((place == 0) || (place == 1))
			{
				nameEnd = line.find(" ");
				if ( nameEnd == std::string::npos)
					nameEnd = line.size();
				if (place == 0)
				{
					start = 1;
					end = nameEnd;
				}
				else
				{
					start = nameEnd;
					end = line.size();
					if (start != end)
						++start;
				}
			}
			else
			{
				start = 1;
				end = line.size();
			}
			bool found = std::regex_search(line.begin()+start, line.begin()+end, m);
			auto tokens = split(line, " ");

			if ((found && keep) || ((!found) && (!keep)))
			{
				auto nameEnd = line.find(" ");
				if (nameEnd == string::npos)
					seqs_.emplace_back(new Sequence(line.substr(1)));
				else
					seqs_.emplace_back(new Sequence(line.substr(1,nameEnd-1), "", line.substr(nameEnd+1)));
				p = seqs_.back().get();
			}
			else
				p = nullptr;
		}
		else
		{
			if (p!= nullptr)
				p->append(line);
		}
	}
}

void
SequenceSet::readFasta_(AlgorithmPack::Input &inS, map<string, int> &names, bool keep)
{
	std::string line;
	auto itEnd = names.end();
	auto it = names.begin();
	Sequence *p = nullptr;
	//auto &indexS = inS.get();
	while (getline(inS, line))
	{
		if (line.empty())
			continue;
		if (line[0] == '>')
		{
			auto tokens = split(line, " ");
			if (names.empty() || (((it = names.find(tokens[0].substr(1))) != itEnd) && keep) || (((it = names.find(tokens[0].substr(1))) == itEnd) && (!keep)))
			{
				if ((!names.empty()) && (keep))
					++it->second;
				auto nameEnd = line.find(" ");
				if (nameEnd == string::npos)
					seqs_.emplace_back(new Sequence(line.substr(1)));
				else
					seqs_.emplace_back(new Sequence(line.substr(1,nameEnd-1), "", line.substr(nameEnd+1)));
				p = seqs_.back().get();
			}
			else
				p = nullptr;
		}
		else
		{
			if (p!= nullptr)
				p->append(line);
		}
	}
}

void
SequenceSet::readFasta_(AlgorithmPack::Input &inS, map<string, int> &names, const fs::path &index, bool keep)
{
	std::string line;
	auto itEnd = names.end();
	auto it = names.begin();
	AlgorithmPack::Input indexF(index);
	//auto &indexS = indexF.get();
	std::map<unsigned int long, string> indexMap;
	while (getline(indexF, line))
	{
		auto tokens = split(line, "\t");
		//if (names.empty() || (names.find(tokens[0]) != itEnd))
	    if (names.empty() || (((it = names.find(tokens[0])) != itEnd) && keep) || (((it = names.find(tokens[0])) == itEnd) && (!keep)))
			indexMap.emplace(std::stoul(tokens[1]), tokens[0]);
	}
	indexF.close();
	Sequence *p = nullptr;
	for (auto &pair : indexMap)
	{
		++names.find(pair.second)->second;
		inS.seekg(pair.first, ios_base::beg);
		getline(inS, line);
		auto nameEnd = line.find(" ");
		if (nameEnd == string::npos)
			seqs_.emplace_back(new Sequence(line.substr(1)));
		else
			seqs_.emplace_back(new Sequence(line.substr(1,nameEnd-1), "", line.substr(nameEnd+1)));
		p = seqs_.back().get();
		while (getline(inS, line))
		{
			if (line[0] == '>')
				break;
			else
				p->append(line);
		}
	}
}


void
SequenceSet::readFasta_(AlgorithmPack::Input &inS, const std::string &re, const fs::path &index, bool keep)
{
	regex m(re);
	std::string line;
	AlgorithmPack::Input indexF(index);
	//auto &indexS = indexF.get();
	std::map<unsigned int long, string> indexMap;
	while (getline(indexF, line))
	{
		auto tokens = split(line, "\t");
		//if (std::regex_match(tokens[0], m))
		if ((std::regex_match(tokens[0], m) && (keep)) || ((!std::regex_match(tokens[0], m)) && (!keep)))
			indexMap.emplace(std::stoul(tokens[1]), tokens[0]);
	}
	indexF.close();
	Sequence *p = nullptr;
	for (auto &pair : indexMap)
	{
		inS.seekg(pair.first, ios_base::beg);
		getline(inS, line);
		auto nameEnd = line.find(" ");
		if (nameEnd == string::npos)
			seqs_.emplace_back(new Sequence(line.substr(1)));
		else
			seqs_.emplace_back(new Sequence(line.substr(1,nameEnd-1), "", line.substr(nameEnd+1)));
		p = seqs_.back().get();
		while (getline(inS, line))
		{
			if (line[0] == '>')
				break;
			else
				p->append(line);
		}
	}
}


void
SequenceSet::write(const boost::filesystem::path &outfile, bool noComments, std::ios_base::openmode mode)
{
	AlgorithmPack::Output
	outF(outfile, mode);
	auto &outS = outF.get();
	size_t segLength = 70;
	size_t nElems = seqs_.size();
	for (size_t i=0; i<nElems; ++i)
	{
		if (seqs_[i]->comment().empty() || noComments)
			outS << ">" << seqs_[i]->name() << "\n";
		else
			outS << ">" << seqs_[i]->name() << " " << seqs_[i]->comment() << "\n";
		size_t nLength = seqs_[i]->size();
		auto s = seqs_[i]->seq();
		for (size_t j=0; j<nLength; j+=segLength)
			outS << s.substr(j, segLength) << "\n";
	}
	outF.close();
}


void
makeIndex(const fs::path &inFile, const fs::path &outFile)
{
	AlgorithmPack::Input inS(inFile);
	AlgorithmPack::Output outS(outFile);
	string line;
	auto &instream = inS.get();
	auto &outstream = outS.get();
	unsigned long int pos = instream.tellg();
	while(getline(instream, line))
	{
		if (line[0] == '>')
		{
			vector<string> tokens = split(line, " ");
			outstream << tokens[0].substr(1) << "\t" << to_string(pos) << "\n";
		}
		pos = instream.tellg();
	}
	inS.close();
	outS.close();
}



} /* namespace BBL */
