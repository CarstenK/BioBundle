/*
 * Feature.hpp
 *
 *  Created on: 8 Apr 2016
 *      Author: Carsten Kemena
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Carsten Kemena
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

#ifndef SRC_LIB_OTHER_FEATURE_HPP_
#define SRC_LIB_OTHER_FEATURE_HPP_



#include<string>
#include<map>
#include<limits>
#include<iostream>
#include<vector>

#include "../utility/utility.hpp"
#include "../external/Output.hpp"
namespace BBL
{



struct Feature
{
private:
	static const std::map<std::string, int> order;// = {{"gene", 0}, {"mRNA", 1}, {"transcript", 1}, {"exon", 2}, {"intron", 2}, {"CDS", 3}, {"cds", 3}};

public:
	std::string seqID;
	std::string source;
	std::string type;
	size_t start;
	size_t end;
	bool hasScore;
	double score;
	char strand;
	char phase;
	std::map<std::string, std::string> attributes;
	std::vector<Feature> children;

	Feature();
	Feature(std::string &line);
	Feature( const Feature & ) = default;
 	Feature( Feature && ) = default;
    Feature & operator= ( const Feature & ) = default;
	Feature & operator= ( Feature && ) = default;
	~Feature();




    bool operator<(const Feature &other) const
    {
		if (this->seqID != other.seqID)
			return (this->seqID < other.seqID);

		if (this->strand != other.strand)
			return (this->strand < other.strand);

		if ((order.find(this->type) != order.end()) && (order.find(other.type) != order.end()))
		{
			if (this->type != other.type)
				return order.at(this->type) < order.at(other.type);
		}

		if (this->start == other.start)
			return this->end > other.end;

		return this->start < other.start;
    }


	void print(AlgorithmPack::Output &outS)
	{
		outS << seqID << "\t" << source << "\t" << type<< "\t" << start << "\t"
		<< end << "\t";
		if (hasScore)
			outS << score;
		else
			outS << ".";
		outS << "\t" << strand << "\t" << phase << "\t";
		for (auto &tmp : attributes)
			outS << tmp.first << "=" << tmp.second << ";";
		outS << "\n";
	}


	// ctg123 . mRNA 1050 9000 . + . ID=mRNA00001;Parent=gene00001;Name=EDEN.1 5 c
};


//bool featureCompare (const Feature &i, const Feature &j);



} // namespace BBL


#endif
