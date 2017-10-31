/*
 * seqExtract.cpp
 *
 *  Created on: Sep 17, 2015
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
#include "./lib/seq/Alphabets.hpp"
#include "./lib/utility/utility.hpp"
#include "./lib/external/Input.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace AP = AlgorithmPack;

using namespace std;
using namespace std::placeholders;

typedef	multimap<string, vector< pair<size_t, size_t> > > Targets;


bool
minLength(size_t length, const BBL::Sequence &seq)
{
	return (seq.size() >= length);
}

bool
maxLength(size_t length, const BBL::Sequence &seq)
{
	return (seq.size() <= length);
}


bool
parseExtractionLine(const vector<string> &extractLine, set<string> &names, map<string, size_t> &order, Targets &targets, std::string delimiter)
{
	bool subsection = false;
	map<string, Targets>::iterator it;
	Targets t;
	size_t num = 0;
	for (auto elem : extractLine)
	{
		vector< pair<size_t, size_t> > positions;
		if (elem.find(':') == std::string::npos)
		{
			names.insert(elem);
			if (order.count(elem) == 0)
				order[elem] = num++;
			targets.emplace(elem, positions);
		}
		else
		{
			auto tokens = BBL::split(elem, ",-" + delimiter);

			size_t nElems = tokens.size();
			for (size_t i=1; i<nElems; i+=2)
			{
				subsection=true;
				positions.emplace_back(stoi(tokens[i])-1, stoi(tokens[i+1])-1);
			}
			names.insert(tokens[0]);
			if (order.count(tokens[0]) == 0)
				order[tokens[0]] = num++;
			targets.emplace(tokens[0], positions);
		}
	}
	return subsection;
}



void
randExtract(BBL::SequenceSet &set, BBL::SequenceSet &newSet, size_t nSeqs, const string &seed)
{
	std::random_device rd;
	std::mt19937 generator;
	if (seed.empty())
		generator.seed(rd());
	else
		generator.seed(stoi(seed));

    vector<int> numbers;
    size_t setSize = set.size();
    numbers.reserve(setSize);
    for (size_t i=0; i<setSize; ++i)
    	numbers.push_back(i);

    for (size_t i = 0; i<nSeqs; ++i)
    {
    	std::uniform_int_distribution<int> dist(0, numbers.size()-1);
    	size_t num = dist(generator);
    	newSet.emplace_back(set[numbers[num]]);
    	numbers[num] = numbers[numbers.size()-1];
    	numbers.resize(numbers.size()-1);
    }
}


void
revComp(BBL::SequenceSet &set)
{
	size_t nSeqs = set.size();
	for (size_t i=0; i<nSeqs; ++i)
	{
		BBL::reverseComplement(set[i]);
		set[i].name(set[i].name()+":r");
	}
}


void
translate(BBL::SequenceSet &set, const string &translationTable)
{
	map<string, short> tTable = { {"standard",0}, {"mitVerte", 1}, {"mitYeast", 2}, {"protozoan",3}, {"mitInverte",4} };
	size_t nSeqs = set.size();
	for (size_t i=0; i<nSeqs; ++i)
	{
		if ((set[i].size()% 3) != 0)
			throw std::runtime_error("Length of sequence '" + set[i].name() + "' is not a multiple of three!" );
		BBL::translate(set[i], BBL::getTranslationTable(tTable[translationTable]));
	}
}

void
funcExtract(BBL::SequenceSet &set, BBL::SequenceSet &newSet, const vector<string> &functionStrings)
{
	// scan
	vector<function<bool (const BBL::Sequence&)> > functions;
	size_t val;
	for (const auto &func : functionStrings)
	{
		if (func[0] == '>')
		{
			val = stoi(func.substr(1))+1;
			functions.push_back(std::bind(minLength, val, _1));
		}
		if (func[0] == '<')
		{
			val = stoi(func.substr(1))-1;
			functions.push_back(std::bind(maxLength, val, _1));
		}
	}
	size_t nSeqs = set.size();
	for (size_t i=0; i<nSeqs; ++i)
	{
		bool keep = true;
		for (const auto &func : functions)
		{
			if (!func(set[i]))
				keep = false;
		}
		if (keep)
		{
			newSet.emplace_back(set[i]);
		}
	}
}


void
readNamesFile(const fs::path &namesFile, vector<string> &extractLines, char delimiter, size_t column)
{
	AP::Input in(namesFile);
	string line;
	string delims = "";
	delims.push_back(delimiter);
	while (getline(in, line))
	{
		if (!line.empty())
		{
			auto tokens = BBL::split(line, delims);
			extractLines.emplace_back(tokens[column]);
		}
	}
}


void
readInputFile(const fs::path &extractFile, vector<fs::path> &seqFiles, vector<fs::path> &indexFiles)
{
	AP::Input in(extractFile);
	string line;
	while (getline(in, line))
	{
		if (!line.empty())
		{
			auto tokens = BBL::split(line, " \t");
			seqFiles.emplace_back(tokens[0]);
			if (tokens.size() == 2)
				indexFiles.emplace_back(tokens[1]);
		}
	}
}


int
main(int argc, char *argv[])
{
	vector<fs::path> seqFiles, indexFiles;
	fs::path inputList;
	bool index;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<vector<fs::path> >(&seqFiles)->multitoken()->value_name("FILE"), "The input file")
		("index,I", po::value<bool>(&index)->zero_tokens()->default_value(false), "Use an index file")
		("indexFile,F", po::value<vector<fs::path> >(&indexFiles)->multitoken()->value_name("FILE"), "The index file")
		("inputList,l", po::value<fs::path>(&inputList)->value_name("FILE"), "File containing input files")
	;

	fs::path outFile;
	bool append;
	bool removeComments;
	bool extractOrder;
	po::options_description outputOpts("Output options");
	outputOpts.add_options()
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
		("append,a", po::value<bool>(&append)->zero_tokens()->default_value(false), "Append sequences to file")
		("remove-comments,c", po::value<bool>(&removeComments)->zero_tokens()->default_value(false), "Remove comments from output")
		("extract-order", po::value<bool>(&extractOrder)->zero_tokens()->default_value(false), "Keeps the order given in the extraction line")
	;

	vector<string> extractLine;
	vector<string> functionStrings;
	size_t numSeqs;
	string seed;
	bool re;
	bool remove;
	size_t column;
	string place;
	fs::path namesFile;
	bool ignoreMissing;
    char delimiterPos, delimiterExtract;
	po::options_description extractOpts("Extract options");
	extractOpts.add_options()
		("extract,e", po::value<vector<string> >(&extractLine)->multitoken(), "The sequence to extract")
		("namesFile,N", po::value<fs::path>(&namesFile)->value_name("FILE"), "File with extraction lines to use")
		("delim-extract,d", po::value<char>(&delimiterExtract)->default_value('\t', "Tab"), "The delimiter to use in the extraction file")
		("column,C", po::value<size_t>(&column)->default_value(1), "The column in the extraction file to use")
		("delim-pos,D", po::value<char>(&delimiterPos)->default_value(':'), "The delimiter to use to seperate name from positions")
		("regex,E", po::value<bool>(&re)->default_value(false)->zero_tokens(), "Interpret extract line as regex")
		("part,p", po::value<string>(&place)->default_value("name"), "The place to look for regex")
		("remove,r", po::value<bool>(&remove)->default_value(false)->zero_tokens(), "Remove the given sequences")
		("numSeqs,n", po::value<size_t>(&numSeqs)->default_value(0), "The number of sequences to extract")
		("seed,s", po::value<string>(&seed), "Seed for random extract function")
		("function,f", po::value<vector<string> >(&functionStrings)->multitoken(), "The function to use for extraction")
		("ignore-missing,m", po::value<bool>(&ignoreMissing)->default_value(false), "Ignore missing sequences")
	;

	bool doRevComp, doTranslate;
	string translationTable;
	po::options_description modifyOpts("Modifying options");
	modifyOpts.add_options()
		("translate,t", po::value<bool>(&doTranslate)->zero_tokens()->default_value(false), "Translate into amino acid")
		("table,T", po::value<string>(&translationTable)->default_value("standard"), "The translation table to use")
		("revComp,R", po::value<bool>(&doRevComp)->zero_tokens()->default_value(false), "Calculate the reverse complement")
	;


	po::options_description all("seqExtract Copyright (C) 2015,2016  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below.");
	all.add(generalOpts).add(outputOpts).add(extractOpts).add(modifyOpts);

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
		if ((!vm.count("in")) && (!vm.count("inputList")))
		{
			throw std::runtime_error("No input files are given!");
		}

	}
	catch(std::exception &e)
	{
		std::cerr << "Error parsing commandline: " << e.what() << endl;
		std::cerr << "Use -h/--help for option information" << endl;
		exit(EXIT_FAILURE);
	}

	if (!namesFile.empty())
		readNamesFile(namesFile, extractLine, delimiterExtract, column-1);

	// read names File
	if (!inputList.empty())
		readInputFile(inputList, seqFiles, indexFiles);

	if ((!indexFiles.empty()) && (seqFiles.size() != indexFiles.size()))
	{
		cerr << "Error! If you provide indexFiles you need to provide them for every sequence file given\n";
		exit(EXIT_FAILURE);
	}


	std::ios_base::openmode mode;
	if (append)
		mode = std::ios_base::app;
	else
		mode = std::ios_base::out;

	short placeN;
	if (place == "name")
		placeN = 0;
	else
	{
		if (place == "comment")
			placeN = 1;
		else
			placeN = 2;
	}

	set<string> nameSet;
	map<string, size_t> order;
	Targets positions;
    string delims = "";
    delims.push_back(delimiterPos);
	bool subsection = parseExtractionLine(extractLine, nameSet, order, positions, delims);
	BBL::SequenceSet set;
	if (!indexFiles.empty())
		index = true;
        std::map<std::string, unsigned int> seqCounter;
	try
	{
		BBL::SequenceSet newSet;
		if (extractOrder)
			newSet.resize(order.size());
		size_t nFiles = seqFiles.size();
		for (size_t i=0; i<nFiles; ++i)
		{
			auto &seqFile = seqFiles[i];
			if (index)
			{
				fs::path indexF;
				if (indexFiles.empty())
				{
					indexF = seqFiles[i];
					indexF.replace_extension("sei");
				}
				else
				{
					indexF = indexFiles[i];
				}

				if (!exists(indexF))
					BBL::makeIndex(seqFile, indexF);
				if (!re)
					set.read(seqFile, nameSet, !remove, indexF, true);
				else
					set.read(seqFile, extractLine[0], placeN, !remove, indexF);
			}
			else
			{
				if (!re)
					set.read(seqFile, nameSet, !remove, "", true);
				else
					set.read(seqFile, extractLine[0], placeN, !remove);
			}

			for (auto &seq : set)
				seqCounter[seq->name()] += 1;

			if (subsection)
			{
				size_t nSeqs=set.size();
				for (size_t i=0; i<nSeqs; ++i)
				{
					auto range = positions.equal_range(set[i].name());

					for (auto it=range.first; it!=range.second; ++it)
					{
						bool first = true;
						auto seq = set[i].seq();
						string name = set[i].name();
						string sequence;
						if (!it->second.empty())
						{
							for (const auto &elem : it->second)
							{
								size_t start = elem.first;
								sequence.append(seq.substr(start,elem.second-start+1));
								name.append((first ? ":" : ",") + to_string(start+1) + "-" + to_string(elem.second+1));
								first = false;
							}
							newSet.emplace_back(BBL::Sequence(std::move(name), std::move(sequence)));
						}
						else
							newSet.emplace_back(BBL::Sequence(set[i].name(), set[i].seq()));
					}
				}

			}
			else
			{
				if (numSeqs != 0)
					randExtract(set, newSet, numSeqs, seed);
				else
				{
					if (!functionStrings.empty())
						funcExtract(set, newSet, functionStrings);
					else
					{
						if (extractOrder)
						{
							size_t nSeqs = set.size();
							for (size_t i=0; i<nSeqs; ++i)
							{
								size_t pos = order[set[i].name()];
								newSet[pos] = std::move(set[i]);
							}
							set.clear();
						}
						else
						{
							size_t nSeqs = set.size();
							for (size_t i = 0; i<nSeqs; ++i)
								newSet.emplace_back(std::move(set[i].name()), std::move(set[i].seq()), std::move(set[i].comment()));
							set.clear();
						}
					}
				}
			}

			if (doRevComp)
				revComp(newSet);
			if (doTranslate)
				translate(newSet, translationTable);

			if (i > 0)
				mode = std::ios_base::app;
		}
		newSet.write(outFile, removeComments, mode);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error! " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	if ((!ignoreMissing) && (!remove) && (!re))
	{
		bool foundProblem = false;
		for (auto &name : nameSet)
		{
			if (seqCounter.count(name) == 0)
			{
				std::cerr << "Warning! Sequence '" << name << "' not found!\n";
				foundProblem = true;
			}
		}
		if (foundProblem)
			return 1;
	}

	return EXIT_SUCCESS;
}
