/*
 * utility.hpp
 *
 *  Created on: Sep 16, 2015
 *      Author: Carsten Kemena
 *		 Email: CarstenK[@]posteo.de
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

#ifndef SRC_UTILITY_UTILITY_HPP_
#define SRC_UTILITY_UTILITY_HPP_

#include <string>
#include <vector>

/**
 * \file utility.hpp
 * \brief This file contains some useful functions.
 */

namespace BBL
{
	/**
	 * \brief Splits a string according to the given delimiters.
	 * @param[in] s The string to split.
	 * @param[in] delimiters The delimiters to use.
	 * @return A vector of the resulting tokens.
	 */
	std::vector<std::string>
	split(const std::string &s, const std::string &delimiters, bool keepEmpty=false);

	/**
	 * \brief Splits a string according to the given delimiters.
	 * @param[in] s The string to split.
	 * @param[in] delimiters The delimiters to use.
	 * @param[out] tokens  A vector to store the resulting tokens.
	 */
	void
	split(const std::string &s, const std::string &delimiters, std::vector<std::string> &tokens, bool keepEmpty=false);

}


#endif /* SRC_UTILITY_UTILITY_HPP_ */
