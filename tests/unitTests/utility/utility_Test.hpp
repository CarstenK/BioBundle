/*
 * utility_TEST.hpp
 *
 *  Created on: Sep 19, 2015
 *      Author: ck
 */


#ifndef UTILITY_TEST_HPP_
#define UTILITY_TEST_HPP_

// C++ header
#include <string>
#include <iostream>
#include <vector>

// other header
#include <boost/test/unit_test.hpp>

// BBL header
#include "../../../src/lib/utility/utility.hpp"

BOOST_AUTO_TEST_SUITE(Utility_Test)

BOOST_AUTO_TEST_CASE( split_Test ) {
	std::string str = "This is a simple test";
	auto vec = BBL::split(str, " ");
	BOOST_CHECK_EQUAL(vec.size(), 5);
	BOOST_CHECK_EQUAL(vec[0], "This");
	BOOST_CHECK_EQUAL(vec[1], "is");
	BOOST_CHECK_EQUAL(vec[2], "a");
	BOOST_CHECK_EQUAL(vec[3], "simple");
	BOOST_CHECK_EQUAL(vec[4], "test");

	std::vector<std::string> vec2;
	BBL::split(str, " ", vec2);
	BOOST_CHECK_EQUAL(vec2.size(), 5);
	BOOST_CHECK_EQUAL(vec2[0], "This");
	BOOST_CHECK_EQUAL(vec2[1], "is");
	BOOST_CHECK_EQUAL(vec2[2], "a");
	BOOST_CHECK_EQUAL(vec2[3], "simple");
	BOOST_CHECK_EQUAL(vec2[4], "test");
}

BOOST_AUTO_TEST_SUITE_END()

#endif /* UTLILITY_TEST_HPP_ */



