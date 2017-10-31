/*
 * Output.cpp
 *
 *  Created on: Sep 13, 2015
 *      Author: CarstenK
 *		 Email: CarstenK[@]posteo.de
 *	 Copyright: 2015
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

#include "Output.hpp"

namespace AlgorithmPack
{


Output::Output()
{
	// TODO Auto-generated constructor stub

}

Output::~Output()
{
	// TODO Auto-generated destructor stub
}

Output::Output(const boost::filesystem::path &fileName, std::ios_base::openmode mode)
{
	open(fileName, mode);
}

void
Output::open(const boost::filesystem::path &fileName, std::ios_base::openmode mode)
{
	if (fileName.empty())
	{
		oS_ = &std::cout;
	}
	else
	{
		if (fileName.extension() == ".gz")
		{
			outS_.open(fileName.string(), mode | std::ios_base::binary);
			outbuf_ = new boost::iostreams::filtering_streambuf<boost::iostreams::output>();
			outbuf_->push(boost::iostreams::gzip_compressor());
			outbuf_->push(outS_);
			oS_ = new std::ostream(outbuf_);
		}
		else
		{
			if (fileName.extension() == ".bz2")
			{
				outS_.open(fileName.string(), mode | std::ios_base::binary);
				outbuf_ = new boost::iostreams::filtering_streambuf<boost::iostreams::output>();
				outbuf_->push(boost::iostreams::bzip2_compressor());
				outbuf_->push(outS_);
				oS_ = new std::ostream(outbuf_);
			}
			else
			{
				outS_.open(fileName.string(), mode);
				oS_ = &outS_;
			}
		}
	}
	// throw an exception if opening fails
	if (outS_.fail())
		throw std::ios_base::failure("Error opening file '" + fileName.string() + "': " +strerror(errno));
}



}

