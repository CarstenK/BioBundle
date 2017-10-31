

#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>

#include "./lib/other/Feature.hpp"
#include "./lib/external/Input.hpp"
#include "./lib/external/Output.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;


void recursive_print(const string &id, map<string, vector<BBL::Feature> > &children, AlgorithmPack::Output &outS, size_t &write_counter)
{
    sort(children[id].begin(), children[id].end());
    for (auto &child_feat : children[id])
    {
        child_feat.print(outS);
        ++write_counter;
        auto it_id = child_feat.attributes.find("ID");
        if (it_id != child_feat.attributes.end())
        {
            auto it = children.find(it_id->second);
            if (it != children.end())
                recursive_print(it_id->second, children, outS, write_counter);
        }
    }
}

int
main(int argc, char *argv[])
{

    fs::path outFile, gffFile;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
        ("in,i", po::value<fs::path>(&gffFile)->value_name("FILE")->required(), "The GFF file")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
	;


	po::options_description all("gffSort Copyright (C) 2017  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
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

    AlgorithmPack::Input inS(gffFile);
    AlgorithmPack::Output outS(outFile);
    std::string line;
    vector<BBL::Feature> features;
    map<string, vector<BBL::Feature> > children;
    size_t feature_counter = 0;
    while (getline(inS, line))
    {
        if ((line.empty()) || (line[0] == '#'))
            continue;
        BBL::Feature feature(line);
        ++feature_counter;
        if (feature.seqID.empty())
            cout << line << "\n";
        auto it = feature.attributes.find("Parent");
        if (it != feature.attributes.end())
            children[it->second].emplace_back(feature);
        else
            features.emplace_back(feature);
    }

    size_t write_counter = 0;
    std::sort(features.begin(), features.end());
    for (auto &feature : features)
    {
        feature.print(outS);
        ++write_counter;
        auto it = feature.attributes.find("ID");
        if (it != feature.attributes.end())
        {
            if (children.find(it->second) != children.end())
                recursive_print(it->second, children, outS, write_counter);
        }
    }

    if (write_counter != feature_counter)
    {
        cerr << "Error! " << feature_counter << " feature read but only " << write_counter << " written! This could indicate an error in your gff structure.\n"; 
        exit(EXIT_FAILURE);
    }


    return EXIT_SUCCESS;
}
