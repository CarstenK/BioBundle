/*
 * Alphabet_Test.hpp
 *
 *  Created on: Sep 6, 2015
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

#ifndef TESTS_SEQ_ALPHABET_TEST_HPP_
#define TESTS_SEQ_ALPHABET_TEST_HPP_

// C++ header
#include <string>

// other header
#include <boost/test/unit_test.hpp>

// BBL header
#include "../../../src/lib/seq/Alphabets.hpp"
#include "../../../src/lib/seq/Sequence.hpp"

BOOST_AUTO_TEST_SUITE(Alphabet_Test)


BOOST_AUTO_TEST_CASE( translate_Test )
{

	std::string dnaSeq = "GCTGCCGCAGCGTTATTGCTTCTCCTACTGCGTCGCCGACGGAGAAGGAAAAAGAATAACATGGATGACTTTTTCTGTTGCCCTCCCCCACCGCAACAGTCTTCCTCATCGAGTAGCGAAGAGACTACCACAACGGGTGGCGGAGGGTGGCATCACTATTACATTATCATAGTTGTCGTAGTGTAATGATAG";
	BBL::Sequence seq("seq1", dnaSeq);

	std::string aaSeq;
	BBL::translate(dnaSeq, BBL::getTranslationTable(0), aaSeq);
	BOOST_CHECK_EQUAL(aaSeq, "AAAALLLLLLRRRRRRKKNNMDDFFCCPPPPQQSSSSSSEETTTTGGGGWHHYYIIIVVVV***");

	BBL::translate(dnaSeq, BBL::getTranslationTable(0));
	BOOST_CHECK_EQUAL(dnaSeq, "AAAALLLLLLRRRRRRKKNNMDDFFCCPPPPQQSSSSSSEETTTTGGGGWHHYYIIIVVVV***");

	BBL::translate(seq, BBL::getTranslationTable(0));
	BOOST_CHECK_EQUAL(seq.seq(), "AAAALLLLLLRRRRRRKKNNMDDFFCCPPPPQQSSSSSSEETTTTGGGGWHHYYIIIVVVV***");

	std::string dnaSeq2 = "ANCATG";
	BBL::Sequence seq2("seq1", dnaSeq2);
	BBL::translate(dnaSeq2, BBL::getTranslationTable(0), aaSeq);
	BOOST_CHECK_EQUAL(aaSeq,"XM");
	BBL::translate(seq2, BBL::getTranslationTable(0));
	BOOST_CHECK_EQUAL(seq2.seq(), "XM");

	dnaSeq = "TTTTTCTTATTGTCTTCCTCATCGTATTACTAATAGTGTTGCTGATGGCTTCTCCTACTGCCTCCCCCACCGCATCACCAACAGCGTCGCCGACGGATTATCATAATGACTACCACAACGAATAACAAAAAGAGTAGCAGAAGGGTTGTCGTAGTGGCTGCCGCAGCGGATGACGAAGAGGGTGGCGGAGGG";
	BBL::translate(dnaSeq, BBL::getTranslationTable(1), aaSeq);
	BOOST_CHECK_EQUAL(aaSeq, "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSS**VVVVAAAADDEEGGGG");

	BBL::translate(dnaSeq, BBL::getTranslationTable(2), aaSeq);
	BOOST_CHECK_EQUAL(aaSeq, "FFLLSSSSYY**CCWWTTTTPPPPHHQQRRRRIIMMTTTTNNKKSSRRVVVVAAAADDEEGGGG");

	BBL::translate(dnaSeq, BBL::getTranslationTable(3), aaSeq);
	BOOST_CHECK_EQUAL(aaSeq, "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG");

	BBL::translate(dnaSeq, BBL::getTranslationTable(4), aaSeq);
	BOOST_CHECK_EQUAL(aaSeq, "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSSSSVVVVAAAADDEEGGGG");
}


BOOST_AUTO_TEST_CASE( complement_Test )
{
	std::string dna="ACGTT";
	BBL::Sequence dnaSeq("seq1", dna);
	BBL::reverseComplement(dnaSeq);
	BOOST_CHECK_EQUAL(dnaSeq.seq(), "AACGT");
}

BOOST_AUTO_TEST_SUITE_END()


#endif /* TESTS_SEQ_ALPHABET_TEST_HPP_ */
