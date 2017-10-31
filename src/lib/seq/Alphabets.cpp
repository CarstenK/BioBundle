/*
 * Alphabet.cpp
 *
 *  Created on: 25.11.2015
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

#include "Alphabets.hpp"

namespace BBL
{

std::map<std::string, char>
getTranslationTable(short table)
{
	// Standard dna to amino acid translation table
	// http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi
	std::map<std::string, char> dna2aa =
	{
		{"TTT", 'F'}, {"TTC", 'F'}, {"TTA", 'L'}, {"TTG", 'L'}, {"CTT", 'L'}, {"CTC", 'L'},
		{"CTA", 'L'}, {"CTG", 'L'}, {"ATT", 'I'}, {"ATC", 'I'}, {"ATA", 'I'}, {"ATG", 'M'},
		{"GTT", 'V'}, {"GTC", 'V'}, {"GTA", 'V'}, {"GTG", 'V'}, {"TCT", 'S'}, {"TCC", 'S'},
		{"TCA", 'S'}, {"TCG", 'S'}, {"CCT", 'P'}, {"CCC", 'P'}, {"CCA", 'P'}, {"CCG", 'P'},
		{"ACT", 'T'}, {"ACC", 'T'}, {"ACA", 'T'}, {"ACG", 'T'}, {"GCT", 'A'}, {"GCC", 'A'},
		{"GCA", 'A'}, {"GCG", 'A'}, {"TAT", 'Y'}, {"TAC", 'Y'}, {"TAA", '*'}, {"TAG", '*'},
		{"CAT", 'H'}, {"CAC", 'H'}, {"CAA", 'Q'}, {"CAG", 'Q'}, {"AAT", 'N'}, {"AAC", 'N'},
		{"AAA", 'K'}, {"AAG", 'K'}, {"GAT", 'D'}, {"GAC", 'D'}, {"GAA", 'E'}, {"GAG", 'E'},
		{"TGT", 'C'}, {"TGC", 'C'}, {"TGA", '*'}, {"TGG", 'W'}, {"CGT", 'R'}, {"CGC", 'R'},
		{"CGA", 'R'}, {"CGG", 'R'}, {"AGT", 'S'}, {"AGC", 'S'}, {"AGA", 'R'}, {"AGG", 'R'},
		{"GGT", 'G'}, {"GGC", 'G'}, {"GGA", 'G'}, {"GGG", 'G'}
	};

	switch (table)
	{
		case 0:
			break;
		case 1:
			// changes in vertebrate mitochondria
			dna2aa["AGA"] = '*';
			dna2aa["AGG"] = '*';
			dna2aa["ATA"] = 'M';
			dna2aa["TGA"] = 'W';
			break;

		case 2:
			// changes in yeast mitochondria
			dna2aa["ATA"] = 'M';
			dna2aa["CTT"] = 'T';
			dna2aa["CTC"] = 'T';
			dna2aa["CTA"] = 'T';
			dna2aa["CTG"] = 'T';
			dna2aa["TGA"] = 'W';

		//	dna2aa.erase("CGA");
		//	dna2aa.erase("CGC");
			break;

		case 3:
			// The Mold, Protozoan, and Coelenterate Mitochondrial Code and the Mycoplasma/Spiroplasma Code
			dna2aa["TGA"] = 'W';
			break;

		case 4:
			// changes for invertebrate mitochondria
			dna2aa["AGA"] = 'S';
			dna2aa["AGG"] = 'S';
			dna2aa["ATA"] = 'M';
			dna2aa["TGA"] = 'W';
			break;

		default:
			throw std::runtime_error("Translation table choice not supported");

	}

	return dna2aa;
}

}
