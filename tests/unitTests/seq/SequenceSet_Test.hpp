/*
 * SequenceSet_Test.hpp
 *
 *  Created on: Sep 9, 2015
 *      Author: CarstenK
 *		 Email: CarstenK[@]posteo.de
 *	 Copyright: 2015
 *
 *  This file is part of BasicBioLib.
 *
 *  BasicBioLib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  BasicBioLib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with BasicBioLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TESTS_SEQ_SEQUENCESET_TEST_HPP_
#define TESTS_SEQ_SEQUENCESET_TEST_HPP_


// C++ header
#include <string>
#include <set>
#include <iostream>

// other header
#include <boost/test/unit_test.hpp>

// BBL header
#include "../../../src/lib/seq/SequenceSet.hpp"


BOOST_AUTO_TEST_SUITE(SequenceSet_Test)





BOOST_AUTO_TEST_CASE( read_Test )
{

	// check reading a whole file
	BBL::SequenceSet set;
	std::string line;
	AlgorithmPack::Input in("../tests/unitTests/seq/data/HBB.fa");
	while (getline(in, line))
		std::cout << line << std::endl;
	set.read(boost::filesystem::path("../tests/unitTests/seq/data/HBB.fa"));

	BOOST_CHECK_EQUAL(set.size(), 15);
	auto seq = set[1];
	BOOST_CHECK_EQUAL(seq.name(), "sp|P02091|HBB1_RAT");
	BOOST_CHECK_EQUAL(seq.comment(), "Hemoglobin subunit beta-1 OS=Rattus norvegicus GN=Hbb PE=1 SV=3");
	BOOST_CHECK_EQUAL(seq.seq(), "MVHLTDAEKAAVNGLWGKVNPDDVGGEALGRLLVVYPWTQRYFDSFGDLSSASAIMGNPKVKAHGKKVINAFNDGLKHLDNLKGTFAHLSELHCDKLHVDPENFRLLGNMIVIVLGHHLGKEFTPCAQAAFQKVVAGVASALAHKYH");
	set.clear();
	BOOST_CHECK_EQUAL(set.size(), 0);

	// check reading certain sequences only
	std::set<std::string> names = {"sp|P68871|HBB_HUMAN", "sp|P02074|HBB_ODOVI", "sp|P02141|HBB4_ONCMY"};
	set.read(boost::filesystem::path("../tests/unitTests/seq/data/HBB.fa"), names);
	BOOST_CHECK_EQUAL(set.size(), 3);
	seq = set[2];
	BOOST_CHECK_EQUAL(seq.name(), "sp|P02074|HBB_ODOVI");
	BOOST_CHECK_EQUAL(seq.seq(), "MLTAEEKAAVTGFWGKVNVDVVGAEALGRLLVVYPWTQRFFEHFGDLSSAGAVMGNPKVKAHGKRVLDAFSEGLKHLDDLKGAFAELSELHCNKLHVDPENFRLLGNVLVVVLARNFGGEFTPLVQADFQKVVAGVANALAHRYH");

	// check reading with index
	set.clear();
	BBL::makeIndex("../tests/unitTests/seq/data/HBB.fa", "HBB.fa.sei");
	set.read("../tests/unitTests/seq/data/HBB.fa", names, "HBB.fa.sei");
	BOOST_CHECK_EQUAL(set.size(), 3);
	seq = set[2];
	BOOST_CHECK_EQUAL(seq.name(), "sp|P02074|HBB_ODOVI");
	BOOST_CHECK_EQUAL(seq.seq(), "MLTAEEKAAVTGFWGKVNVDVVGAEALGRLLVVYPWTQRFFEHFGDLSSAGAVMGNPKVKAHGKRVLDAFSEGLKHLDDLKGAFAELSELHCNKLHVDPENFRLLGNVLVVVLARNFGGEFTPLVQADFQKVVAGVANALAHRYH");

}


BOOST_AUTO_TEST_SUITE_END()



#endif /* TESTS_SEQ_SEQUENCESET_TEST_HPP_ */
