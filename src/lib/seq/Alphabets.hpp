/*
 * Alphabet.hpp
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

#ifndef SRC_SEQ_ALPHABETS_HPP_
#define SRC_SEQ_ALPHABETS_HPP_


// C++ header
#include <algorithm>
#include <locale>
#include <map>
#include <set>
#include <string>

#include <iostream>
/**
 * \brief Namespace of this library.
 */
namespace BBL
{

/**
 * \file Alphabets.hpp
 * \brief This file contains a range of different fixed alphabet sets and translation tables.
 */

/// The DNA alphabet.
const std::set<char> DNA =
{
    'A', 'G', 'C', 'T', 'N', 'X',
	'a', 'g', 'c', 't', 'n', 'x'
};


/// The RNA alphabet.
const std::set<char> RNA =
{
    'A', 'G', 'C', 'U', 'N', 'X',
	'a', 'g', 'c', 'u', 'n', 'x'
};


/// The extended DNA alphabet including summary characters.
const std::set<char> DNAExtended =
{
    'A', 'G', 'C', 'T', 'Y', 'R', 'W', 'S', 'R', 'M', 'A', 'V', 'H', 'B', 'N', 'X',
	'a', 'g', 'c', 't', 'y', 'r', 'w', 's', 'r', 'm', 'a', 'v', 'h', 'b', 'n', 'x'
};


/// The extended RNA alphabet including summary characters.
const std::set<char> RNAExtended =
{
    'A', 'G', 'C', 'U', 'Y', 'R', 'W', 'S', 'R', 'M', 'A', 'V', 'H', 'B', 'N', 'X',
	'a', 'g', 'c', 'u', 'y', 'r', 'w', 's', 'r', 'm', 'a', 'v', 'h', 'b', 'n', 'x'
};


/// The amino acid alphabet.
const std::set<char> aminoAcid =
{
    'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X',
	'a', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x'
};


/// The extended amino acid alphabet including summary characters.
const std::set<char> aminoAcidExtended =
{
    'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S',
    'T', 'V', 'W', 'X', 'B', 'Z',
	'a', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's',
	't', 'v', 'w', 'x', 'b', 'z'
};


/// Table to turn three letter amino acid codes into one letter amino acid codes.
const std::map<std::string, char> three2one =
{
    {"Ala", 'A'}, {"Asx", 'B'}, {"Cys", 'C'}, {"Asp", 'D'}, {"Glu", 'E'}, {"Phe", 'F'},
    {"Gly", 'G'}, {"His", 'H'}, {"Ile", 'I'}, {"Lys", 'K'}, {"Leu", 'L'}, {"Met", 'M'},
    {"Asn", 'N'}, {"Pro", 'P'}, {"Gln", 'Q'}, {"Arg", 'R'}, {"Ser", 'S'}, {"Thr", 'T'},
    {"Val", 'V'}, {"Trp", 'W'}, {"Tyr", 'Y'}, {"Glx", 'Z'}
};


/// Table to turn one letter amino acid code into three letter amino acid codes.
const std::map<char, std::string> one2three =
{
    {'A', "Ala"}, {'B', "Asx"}, {'C', "Cys"}, {'D', "Asp"}, {'E', "Glu"}, {'F', "Phe"},
    {'G', "Gly"}, {'H', "His"}, {'I', "Ile"}, {'K', "Lys"}, {'L', "Leu"}, {'M', "Met"},
    {'N', "Asn"}, {'P', "Pro"}, {'Q', "Gln"}, {'R', "Arg"}, {'S', "Ser"}, {'T', "Thr"},
    {'V', "Val"}, {'W', "Trp"}, {'Y', "Tyr"}, {'Z', "Glx"}
};




const std::map<char, char> complement =
{
	{'A' , 'T'}, {'T' , 'A'}, {'U' , 'A'}, {'G' , 'C'}, {'C' , 'G'}, {'Y' , 'R'}, {'R' , 'Y'},
	{'S' , 'S'}, {'W' , 'W'}, {'K' , 'M'}, {'M' , 'K'}, {'B' , 'V'}, {'D' , 'H'}, {'H' , 'D'},
	{'V' , 'B'}, {'N' , 'N'}, {'a' , 't'}, {'t' , 'a'}, {'u' , 'a'}, {'g' , 'c'}, {'c' , 'g'},
	{'y' , 'r'}, {'r' , 'y'}, {'s' , 's'}, {'w' , 'w'}, {'k' , 'm'}, {'m' , 'k'}, {'b' , 'v'},
	{'d' , 'h'}, {'h' , 'd'}, {'v' , 'b'}, {'n' , 'n'}
};


/**
 * \brief Returns a translation table for DNA to amino acid conversion.
 *
 * All tables were taken from the NCBI (http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi). All table consist of upper case letter only. Ambiguous nucleotides are not handled.
 * The following tables are currently supported:
 *
 * Number  | Content
 * ------------- | -------------
 * 0  | The standard translation table
 * 1  | The vertebrate mitochondria table
 * 2  | The yeast mitochondria table
 *
 * @param table The number of the table to use
 * @return The translation table
 */
std::map<std::string, char>
getTranslationTable(short table);


/**
 * \brief Calculates the reverse complement of a DNA in place.
 * @param seq The sequence to convert.
 */
template<typename INTYPE>
void
reverseComplement(INTYPE &seq)
{
	try
	{
		std::reverse(seq.begin(),seq.end());
		for (char &c : seq)
			c = complement.at(c);
	}
	catch (const std::out_of_range &e)
	{
		throw (std::runtime_error("No complement for given nucleotide!"));
	}
}


/**
 * \brief Translates a DNA into an amino acid sequence.
 * @param dnaSeq The DNA sequence.
 * @param mapping The mapping to use.
 * @param aaSeq The amino acid result.
 */
template<typename INTYPE, typename OUTTYPE>
void
translate(const INTYPE &dnaSeq, const std::map<std::string, char> &mapping, OUTTYPE &aaSeq)
{
	size_t dnaLength = dnaSeq.size();
	aaSeq.clear();
	aaSeq.reserve(dnaLength/3);
	std::string codon;
	codon.resize(3);
	std::map<std::string, char>::const_iterator itM, itMEnd = mapping.end();
	auto itEnd = dnaSeq.end();

	for (auto it=dnaSeq.begin(); it != itEnd; ++it)
	{
		codon[0] = std::toupper(*it);
		codon[1] = std::toupper(*++it);
		codon[2] = std::toupper(*++it);
		if ((itM=mapping.find(codon)) != itMEnd)
			aaSeq.push_back(itM->second);
		else
			aaSeq.push_back('X');
	}
}


/**
 * \brief Translates a DNA into protein in place.
 * @param dnaSeq The DNA sequence.
 * @param mapping The mapping to use.
 */
template<typename INTYPE>
void
translate(INTYPE &dnaSeq, const std::map<std::string, char> &mapping)
{
	size_t dnaLength = dnaSeq.size();
	std::string codon;
	codon.resize(3);
	auto aaIT = dnaSeq.begin();
	auto itEnd = dnaSeq.end();
	std::map<std::string, char>::const_iterator itM, itMEnd = mapping.end();
	for (auto it = dnaSeq.begin(); it!=itEnd; ++it)
	{
		codon[0] = std::toupper(*it);
		codon[1] = std::toupper(*++it);
		codon[2] = std::toupper(*++it);
		if ((itM=mapping.find(codon)) != itMEnd)
			*(aaIT++) = itM->second;
		else
			*(aaIT++) = 'X';

	}
	dnaSeq.resize(dnaLength/3);
}


} // BBL


#endif /* SRC_SEQ_ALPHABETS_HPP_ */
