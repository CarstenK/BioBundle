#include "Feature.hpp"

namespace BBL
{

Feature::Feature() {
	// TODO Auto-generated constructor stub
}

Feature::~Feature() {
	// TODO Auto-generated destructor stub
}

const std::map<std::string, int> Feature::order = {{"gene", 0}, {"mRNA", 1}, {"transcript", 1}, {"exon", 2}, {"intron", 2}, {"CDS", 3}, {"cds", 3}};


Feature::Feature(std::string &line)
{
	auto tokens = split(line, "\t", false);
	seqID = tokens[0];
	source = tokens[1];
	type = tokens[2];
	start = std::stol(tokens[3]);
	end = std::stol(tokens[4]);
	if ((tokens[5].size() == 1) && (tokens[5][0] == '.'))
		hasScore = false;
	else
	{
		hasScore = true;
		score = std::stod(tokens[5]);
	}
	strand = tokens[6][0];
	phase = tokens[7][0];

	auto atts = split(tokens[8], ";", false);
	for (auto &sub : atts)
	{
		auto tmp = split(sub, "=", false);
		attributes[tmp[0]] = tmp[1];
	}

	// ctg123 . mRNA 1050 9000 . + . ID=mRNA00001;Parent=gene00001;Name=EDEN.1 5 c
}




}
