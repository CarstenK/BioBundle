/*
 * seqSetInfo.cpp
 *
 *  Created on: 23.10.2015
 *      Author: ck
 */

#include <limits>
#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "./lib/seq/SequenceSet.hpp"
#include "./lib/utility/utility.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;


int
main(int argc, char *argv[])
{
	fs::path inFile, outFile;
	vector<string> names;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<fs::path>(&inFile)->required(), "The input file")
		("out,o", po::value<fs::path>(&outFile), "The output file")
		("names,n", po::value<vector<string> >(&names), "The sequence names")
	;

	bool calcLengths;
	bool calcCharStats;
	po::options_description statOpts("Statistics options");
	statOpts.add_options()
	("lengths,l", po::value<bool>(&calcLengths)->zero_tokens()->default_value(false),"Get sequence lengths")
	("chars,c", po::value<bool>(&calcCharStats)->zero_tokens()->default_value(false),"Get character stats")
	;

	po::options_description all("colCal Copyright (C) 2015  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below.");
	all.add(generalOpts).add(statOpts);
	po::variables_map vm;


	try
	{
		po::store(po::command_line_parser(argc, argv).options(all).run(), vm);
		if (vm.count("help"))
		{
			std::cout << all << "\n";
			exit(EXIT_SUCCESS);
		}
		po::notify(vm);
	}
	catch(std::exception &e)
	{
		std::cerr << "Error parsing commandline: " << e.what() << endl;
		std::cerr << "Use -h/--help for option information" << endl;
		exit(EXIT_FAILURE);
	}

	BBL::SequenceSet seqSet;
	try
	{
		seqSet.read(inFile, names);
	}
	catch (std::exception &e)
	{
		cerr << "ERROR! " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	//length vars;
	size_t minLength = std::numeric_limits<size_t>::max();
	size_t maxLength = 0;
	size_t totalLength = 0;

	// length vars
	map<char, int> charCounter;

	AlgorithmPack::Output out(outFile);
	auto &outS = out.get();

	size_t nSeqs = seqSet.size();
	for (size_t i=0; i<nSeqs; ++i)
	{
		const auto &seq = seqSet[i];

		totalLength += seq.size();
		if (calcLengths)
		{
			outS << seq.name() << "\n";
			outS << "Length: " << seq.size() << "\n";
			if (seq.size() < minLength)
				minLength = seq.size();
			if (seq.size() > maxLength)
				maxLength = seq.size();
		}

		if (calcCharStats)
		{
			for (const char &c : seq)
				++charCounter[c];
		}
	}

	outS.precision(1);
	outS << fixed << "\nSummary:";
	if (calcLengths)
	{
		outS << "\nLength:\n";
		outS << "min length   : " << minLength << "\n";
		outS << "max length   : " << maxLength << "\n";
		outS << "total length : " << totalLength << "\n";
		outS << "avg length   : " << static_cast<double>(totalLength)/seqSet.size() << "\n";
	}

	if (calcCharStats)
	{
		outS << "\nChar stat:\n";
		for (auto &pair : charCounter)
			outS << pair.first << " " << pair.second << " " << (100*static_cast<double>(pair.second))/totalLength << "%\n";

	}



	return EXIT_SUCCESS;
}
