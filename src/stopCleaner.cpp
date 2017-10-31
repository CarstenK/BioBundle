/*
 * cleanProt.cpp
 *
 *  Created on: 13.02.2017
 *      Author: Carsten Kemena
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
#include "./lib/external/Output.hpp"
#include "./lib/seq/Sequence.hpp"
#include "./lib/seq/SequenceSet.hpp"
#include "./lib/utility/utility.hpp"

// other
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;


void
read_names_file(const fs::path &keepFile, set<string> &names2keep)
{
    AlgorithmPack::Input inF(keepFile);
    string line;

    while (getline(inF, line))
    {
        if (line.empty() || (line[0] == '#'))
            continue;
        names2keep.emplace(std::move(line));
    }

}


int
main(int argc, char *argv[])
{
    fs::path outFile, protFile, outList, keepFile;
    string stopChars;
    std::vector<string> keep;
    char replace;
    bool no_remove_final_stops, remove_pseudogenes;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
        ("in,i", po::value<fs::path>(&protFile)->value_name("FILE"), "The input file")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
        ("no-final-stop-removal", po::value<bool>(&no_remove_final_stops)->zero_tokens()->default_value(false), "Do not remove the final stop")
        ("remove-pseudogenes,r", po::value<bool>(&remove_pseudogenes)->zero_tokens()->default_value(false), "Remove pseudogenes")
        ("stop,s", po::value<string>(&stopChars)->default_value(".*"), "The stop chars to use")
        ("list,l", po::value<fs::path>(&outList)->value_name("FILE"), "Produce list with removed genes")
        ("keep,k", po::value<vector<string>>(&keep)->multitoken(), "Keep sequences with these IDs")
        ("keepFile,K", po::value<fs::path>(&keepFile)->value_name("FILE"), "File width sequence IDs to keep")
        ("replace,R", po::value<char>(&replace), "Replace stops with this char")
	;

	po::options_description all("stopCleaner Copyright (C) 2017  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
	all.add(generalOpts);
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

    try
    {
        set<string> names2keep;
        for (auto &name: keep)
            names2keep.emplace(std::move(name));
        if (!keepFile.empty())
            read_names_file(keepFile, names2keep);

        BBL::SequenceSet seqSet;
        seqSet.read(protFile);
        vector<string> removedNames;

        BBL::SequenceSet newSeqSet;
        auto nSeqs = seqSet.size();
        for (size_t i=0; i<nSeqs; ++i)
        {
            auto &seq = seqSet[i];
            if (!no_remove_final_stops)
            {
                auto found = stopChars.find(seq[seq.size()-1]);
                while (found != std::string::npos)
                {
                    seq.resize(seq.size()-1);
                    found = stopChars.find(seq[seq.size()-1]);
                }
            }

            auto pos = seq.seq().find_first_of(stopChars);
            if ((!remove_pseudogenes) || ((pos == string::npos) && (!no_remove_final_stops))
                || (((pos == seq.size()-1) || (pos == string::npos)) && (no_remove_final_stops)))
                newSeqSet.emplace_back(std::move(seqSet[i]));
            else
            {
                if (names2keep.find(seq.name()) != names2keep.end())
                {
                    names2keep.erase(seq.name());
                    newSeqSet.emplace_back(std::move(seqSet[i]));
                }
                else
                    removedNames.emplace_back(seq.name());
            }
        }

        for (auto name : names2keep)
            cerr << "Warning! Sequence '" << name << "' not found." << endl;
        if (!outList.empty())
        {
            AlgorithmPack::Output outF(outList);
            for (auto &name : removedNames)
                outF << name << "\n";
            outF.close();
        }

        if (vm.count("replace"))
        {
            for (auto &seq : newSeqSet)
            {
                for (char c : stopChars)
                    std::replace( seq->begin(), seq->end(), c, replace);
            }
        }
        newSeqSet.write(outFile);
    }
	catch (std::exception &e)
	{
		std::cerr << "Error! " << e.what() << "\n";
		return EXIT_FAILURE;
	}
}
