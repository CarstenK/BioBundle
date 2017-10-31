/*
 * isoformCleaner.cpp
 *
 *  Created on: 09.10.2015
 *      Author: ck
 */

// C header
#include <cstdlib>

// C++ header
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>


// BioSeqDataLib
#include "./lib/external/Input.hpp"
#include "./lib/seq/Sequence.hpp"
#include "./lib/seq/SequenceSet.hpp"
#include "./lib/utility/utility.hpp"

// other
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;


map<string, std::pair<size_t, size_t> >
nameFiltering(BBL::SequenceSet &proteinSet, char splitChar)
{
	size_t nProteins = proteinSet.size();
	map<string, std::pair<size_t,size_t> > cleaned;
	for (size_t i=0; i<nProteins; ++i)
	{
		auto &protein = proteinSet[i];
		const string protName = protein.name();
		size_t pos = protName.rfind(splitChar);
		if (pos == string::npos)
			cerr <<"Warning: " << protName << " has no character to split\n";
		string geneName = protName.substr(0,pos);
		auto it = cleaned.find(geneName);
		if (it == cleaned.end())
			cleaned[geneName] = pair<size_t,size_t>(i, protein.size());
		else
		{
			if (it->second.second < protein.size())
				it->second = pair<size_t,size_t>(i, protein.size());
		}
	}
	return cleaned;
}


map<string, std::pair<size_t, size_t> >
gffFiltering(BBL::SequenceSet &proteinSet, const string &gffFile, const string &type, const string &id_field)
{
	map<string, std::pair<size_t, size_t> > nameSize;
	size_t nSeqs = proteinSet.size();
	for (size_t i=0; i<nSeqs; ++i)
		nameSize[proteinSet[i].name()] = pair<size_t, size_t>(i, proteinSet[i].size());

	map<string, std::pair<size_t, size_t> > cleaned;
	AlgorithmPack::Input inF(gffFile);
	auto &inS = inF.get();
	string line;
	while (getline(inS, line))
	{
		if ((!line.empty()) && (line[0] != '#'))
		{
			auto tokens = BBL::split(line, "\t");
			if (tokens[2] == type)
			{
				auto tokens2 = BBL::split(tokens[8], " ;,=");
				size_t nElems = tokens2.size();
				size_t i;
				string parent, id;
				for (i=0; i<nElems; ++i)
				{
					if (tokens2[i] == "Parent")
						parent = tokens2[++i];
					if (tokens2[i] == id_field)
						id = tokens2[++i];
				}

				auto it = cleaned.find(parent);
				if (it == cleaned.end())
					cleaned[parent] = nameSize.at(id);
				else
				{
					if (it->second.second < nameSize.at(id).second)
						it->second = nameSize.at(id);
				}
			}
		}
	}
	inF.close();
	return cleaned;
}


map<string, std::pair<size_t, size_t> >
regexFiltering(BBL::SequenceSet &proteinSet, string regexStr, bool searchComment, bool searchName)
{
	size_t nProteins = proteinSet.size();
	map<string, std::pair<size_t,size_t> > cleaned;
	const regex e (regexStr);
	smatch what;
	for (size_t i=0; i<nProteins; ++i)
	{
		auto &protein = proteinSet[i];
		const string protName = protein.name();
		string target;
		if (searchComment)
			target = protein.comment();
		else
		{
			if (searchName)
				target = protName;
			else
				target = protName + " " + protein.comment();
		}
        bool found = regex_search(target, what, e);
		string geneName;
		if (found)
		{
			geneName = what[1];
		}
		else
		{
			geneName = protName;
			cerr <<"Warning: Regex didn't match " << target << "\n";
		}

		auto it = cleaned.find(geneName);
		if (it == cleaned.end())
			cleaned[geneName] = pair<size_t,size_t>(i, protein.size());
		else
		{
			if (it->second.second < protein.size())
				it->second = pair<size_t,size_t>(i, protein.size());
		}
	}
	return cleaned;
}


void removeIsoforms(const map<string, std::pair<size_t, size_t> > &cleaned, BBL::SequenceSet &proteinSet, BBL::SequenceSet &outSet)
{
	for (auto &isoform : cleaned)
		outSet.emplace_back(std::move(proteinSet[isoform.second.first]));
}

int
main(int argc, char *argv[])
{
	string proteinFile, outFile;
	char splitChar;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<string>(&proteinFile)->value_name("FILE"), "The sequence file")
		("out,o", po::value<string>(&outFile)->value_name("FILE"), "The output file")
		("splitchar,s", po::value<char>(&splitChar), "The split character to use")
	;

	string regex, preset;
	bool searchComment, searchName;
	po::options_description regexOpts("Regex options");
	regexOpts.add_options()
		("regular,r", po::value<string>(&regex), "Regular expression")
		("name,n", po::value<bool>(&searchName)->default_value(false)->zero_tokens(), "Search name only")
		("comment,c", po::value<bool>(&searchComment)->default_value(false)->zero_tokens(), "Search comment only")
		("preset,p", po::value<string>(&preset), "Preset regex")
	;

	string gffFile, type, id_field;
	po::options_description gffOpts("Regex options");
	gffOpts.add_options()
		("gff,g", po::value<string>(&gffFile)->value_name("FILE"), "The gffFile")
		("type,t", po::value<string>(&type)->default_value("mRNA"), "The type to use")
		("id-field,f", po::value<string>(&id_field)->default_value("ID"), "The field to use as protein name")
	;

	po::options_description all("isoformCleaner Copyright (C) 2015,2016  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
	all.add(generalOpts).add(regexOpts).add(gffOpts);
	po::variables_map vm;

	try
	{
		po::store(po::command_line_parser(argc, argv).options(all).run(), vm);
		if (vm.count("help"))
		{
			cout << all<< "\n";
			return EXIT_SUCCESS;
		}
		po::notify(vm);
	}
	catch(std::exception &e)
	{
		std::cerr << "Error parsing commandline: " << e.what() << endl;
		std::cerr << "Use -h/--help for option information" << endl;
		exit(EXIT_FAILURE);
	}


	BBL::SequenceSet proteinSet;

	try
	{
		proteinSet.read(proteinFile);
	}
	catch(std::ios_base::failure &exception)
	{
		cout << "Error opening file \"" << proteinFile << "\": " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}


	BBL::SequenceSet outSet;

	map<string, std::pair<size_t, size_t> > names2keep;

	if (gffFile.empty())
	{
		if (!preset.empty())
		{
			map<string, string> presetRegex = {{"flybase","parent=(FBgn[^ ,]+,)"}, {"gene"," gene[:=]\\s*([\\S]+)[\\s]*"}};
			regex = presetRegex[preset];
			if (regex.empty())
			{
				cerr << "Error: Preset '" << preset << "' is unknown!\n";
				exit(EXIT_FAILURE);
			}
		}
		if (!regex.empty())
		{
			string expandedRegex = ".*" + regex + ".*";
			names2keep = regexFiltering(proteinSet, expandedRegex, searchComment, searchName);
		}
		else
			names2keep = nameFiltering(proteinSet, splitChar);
	}
	else
		names2keep = gffFiltering(proteinSet, gffFile, type, id_field);

	removeIsoforms(names2keep, proteinSet, outSet);
	try
	{
		outSet.write(outFile);
	}
	catch (std::ios_base::failure &exception)
	{
		cout << "Error opening file \"" << outFile << "\": " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
