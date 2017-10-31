/*
 * Sequenc_Test.hpp
 *
 *  Created on: 29.09.2015
 *      Author: ck
 */

#ifndef TESTS_SEQ_SEQUENCE_TEST_HPP_
#define TESTS_SEQ_SEQUENCE_TEST_HPP_

// C++ header
#include <string>
#include <set>
#include <iostream>

// other header
#include <boost/test/unit_test.hpp>

// BBL header
#include "../../../src/lib/seq/Sequence.hpp"


BOOST_AUTO_TEST_SUITE(Sequence_Test)





BOOST_AUTO_TEST_CASE( Sequence_Test )
{
	BBL::Sequence seq("first", "ACGT");
	BOOST_CHECK_EQUAL(seq.name(), "first");
	BOOST_CHECK_EQUAL(seq.size(), 4);
}


BOOST_AUTO_TEST_SUITE_END()



#endif /* TESTS_SEQ_SEQUENCE_TEST_HPP_ */
