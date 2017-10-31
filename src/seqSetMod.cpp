/*
 * seqSetMod.cpp
 *
 *  Created on: 29 Mar 2016
 *      Author: Carsten Kemena
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


#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "./lib/seq/SequenceSet.hpp"
#include "./lib/utility/utility.hpp"
#include "./lib/seq/Alphabets.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;


void
readRenameFile(const fs::path &renameFile, pair<unsigned int, unsigned int> columns, char delimiter, std::map<std::string, std::string> &mapping)
{
	std::string line;
	AlgorithmPack::Input indexF(renameFile);
	string del = "";
	del.push_back(delimiter);

	while (getline(indexF, line))
	{
		auto tokens = BBL::split(line, del);
		mapping[tokens[columns.first]] = tokens[columns.second];
	}
}


int
main(int argc, char *argv[])
{
	fs::path inFile, outFile;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<fs::path>(&inFile)->required()->value_name("FILE"), "The input file")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
	;

	bool rmFinalStop;
	std::pair<unsigned int, unsigned int> columns;
	fs::path renameFile;
	std::string removeFromName;
	char delimiter;
	po::options_description specialOpts("Modifying options");
	specialOpts.add_options()
		("rmFStop,s", po::value<bool>(&rmFinalStop)->zero_tokens()->default_value(false), "Remove the final stops")
		("renaming,r", po::value<fs::path>(&renameFile)->value_name("FILE"), "File containing renaming matches")
		("currentVal,c", po::value<unsigned int>(&columns.first)->default_value(1), "The column with the current value")
		("newVal,n", po::value<unsigned int>(&columns.second)->default_value(2), "The column with the new value")
		("delimiter,d", po::value<char>(&delimiter)->default_value('\t'), "The delimiter to use")
		("removeFromName,R", po::value<std::string>(&removeFromName), "The name part to remove")
	;


	bool translateSeq;
	fs::path cdsOutFile;
	bool cutSeqs;
	po::options_description translateOpts("Translate options");
	translateOpts.add_options()
		("translate,t", po::value<bool>(&translateSeq)->zero_tokens()->default_value(false), "Translate into amino acids")
		("cut,C", po::value<bool>(&cutSeqs)->zero_tokens()->default_value(false), "Cut of sequence if not a multiple of three")
		("cds-out", po::value<fs::path>(&cdsOutFile), "Out put CDS files" )
	;

	po::options_description all("seqExtract Copyright (C) 2015,2016  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below.");
	all.add(generalOpts).add(specialOpts).add(translateOpts);

	po::variables_map vm;

	try
	{
		po::store(po::command_line_parser(argc, argv).options(all).run(), vm);
		if (vm.count("help"))
		{
			std::cout << all << "\n";
			return false;
		}
		po::notify(vm);
	}
	catch(std::exception &e)
	{
		std::cerr << "Error parsing commandline: " << e.what() << endl;
		std::cerr << "Use -h/--help for option information" << endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		BBL::SequenceSet set(inFile);
		size_t nSeqs = set.size();
		if (!renameFile.empty())
		{
			columns.first -=1;
			columns.second -= 1;
			std::map<std::string, std::string> mapping;
			readRenameFile(renameFile, columns, delimiter, mapping);
			auto itEnd = mapping.end();
			size_t count = 0;
			for (auto &seq : set)
			{
				auto it = mapping.find(seq->name());
				if (it != itEnd)
				{
					seq->name(it->second);
					++count;
				}
			}

			if (count != mapping.size())
				cerr << "Warning! Not all patterns were used" << endl;
		}

		if (!removeFromName.empty())
		{
			size_t length = removeFromName.size();
			for (size_t i=0; i<nSeqs; ++i)
			{
				auto &seq = set[i];
				std::string name = seq.name();
				size_t pos = name.find(removeFromName);
				if (pos != std::string::npos)
					name.erase(pos, length);
				seq.name(name);
			}
		}

		BBL::SequenceSet set2;
		if (translateSeq)
		{
			auto tTable = BBL::getTranslationTable(0);
			for (auto &seq : set)
			{
				auto rest = seq->size() % 3;
				if (rest != 0)
				{
					if (cutSeqs)
						seq->resize(seq->size()-rest);
					else
					{
						std::cerr << "The length of sequence '" << seq->name() << "' is not a multiple of three!" << std::endl;
						return (EXIT_FAILURE);
					}
				}
				set2.emplace_back(seq->name(), "");
				BBL::translate(*seq, tTable, set2[set2.size()-1]);
			}
			std::swap(set2, set);
		}


		if (rmFinalStop)
		{
			std::string stopChars = ".*";
			for (size_t i=0; i<nSeqs; ++i)
			{
				char c = set[i][set[i].size()-1];
				if (stopChars.find(c) != std::string::npos)
				{
					set[i].resize(set[i].size()-1);
					if (!cdsOutFile.empty())
						set2[i].resize(set2[i].size()-3);
				}
			}
		}

		if (!cdsOutFile.empty())
			set2.write(cdsOutFile);
		set.write(outFile);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error! " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
