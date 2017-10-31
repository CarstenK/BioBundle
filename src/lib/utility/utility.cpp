/*
 * utilit.cpp
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


#include "utility.hpp"

namespace BBL
{

	std::vector<std::string>
	split(const std::string &s, const std::string &delimiters, bool keepEmpty)
	{
		size_t start=0;
		size_t end=s.find_first_of(delimiters);
		std::vector<std::string> output;
		size_t len=s.size();
		while (start != len)
		{
			if ((end-start >0) || keepEmpty)
				output.emplace_back(s.substr(start, end-start));

			if (end == std::string::npos)
				break;
			start=end+1;
			end = s.find_first_of(delimiters, start);
		}

		return output;
	}

	void
	split(const std::string &s, const std::string &delimiters, std::vector<std::string> &tokens, bool keepEmpty)
	{
		size_t start=0;
		size_t end=s.find_first_of(delimiters);
		size_t len=s.size();
		while (start != len)
		{
			if ((end-start >0) || keepEmpty)
				tokens.emplace_back(s.substr(start, end-start));

			if (end == std::string::npos)
				break;

			start=end+1;
			end = s.find_first_of(delimiters, start);
		}
	}
}
