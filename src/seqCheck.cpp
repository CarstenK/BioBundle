/*
 * seqCheck.cpp
 *
 *  Created on: 2 Apr 2016
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
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "./lib/seq/SequenceSet.hpp"
#include "./lib/seq/Alphabets.hpp"
#include "./lib/utility/utility.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;
using namespace std::placeholders;



int
main(int argc, char *argv[])
{
	fs::path inFile;
	std::string type;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<fs::path>(&inFile)->required(), "The input file")
		("type,t", po::value<string>(&type)->required(), "The sequence type")
	;



	po::options_description all("seqExtract Copyright (C) 2015  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below.");
	all.add(generalOpts);

	po::variables_map vm;

	try
	{
		po::notify(vm);
		if (vm.count("help"))
		{
			std::cout << all << "\n";
			return false;
		}
		po::store(po::command_line_parser(argc, argv).options(all).run(), vm);
	}
	catch(std::exception &e)
	{
		std::cerr << "Error parsing commandline: " << e.what() << endl;
		std::cerr << "Use -h/--help for option information" << endl;
		exit(EXIT_SUCCESS);
	}

	set<char> charSet;
	if (type == "amino")
		charSet = BBL::aminoAcid;
	else
	{
		if (type == "aminoE")
			charSet = BBL::aminoAcidExtended;
		else
		{
			if (type == "DNA")
				charSet = BBL::DNA;
			else
			{
				if (type == "DNAE")
					charSet == BBL::DNAExtended;
				else
				{
					std::cerr << "Unknown sequence type\n";
					return EXIT_FAILURE;
				}
			}
		}
	}

	BBL::SequenceSet set(inFile);
	std::set<char> found;
	for (auto &seq : set)
	{
		auto itEnd = seq->end();
		for (auto it = seq->begin(); it!=itEnd; ++it)
		{
			if (charSet.count(*it) == 0)
				found.insert(*it);
		}
		if (!found.empty())
		{
			cout << seq->name() << ":";
			for (auto c : found)
				cout << " " << c;
			cout << "\n";
		}
		found.clear();
	}

	return EXIT_SUCCESS;
}
