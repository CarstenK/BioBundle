/*
 * Input.cpp
 *
 *  Created on: Sep 10, 2015
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

#include "Input.hpp"

namespace AlgorithmPack
{

	Input::Input() : iS_(nullptr), inbuf_(nullptr), isCompressed_(false), active_(false)
	{
	}


	Input::Input(const boost::filesystem::path &fileName)
	{
		open(fileName);
	}


	Input::~Input()
	{
	}


	void
	Input::open(const boost::filesystem::path &fileName)
	{
		isCompressed_=false;
		// check if decompression is needed
		if (fileName.empty())
		{
			iS_ = &std::cin;
			isCin_ = true;
		}
		else
	 	{
			isCin_ = false;
			if (fileName.extension() == ".gz")
			{
				inS_.open(fileName.string().c_str(), std::ios_base::in | std::ios_base::binary);
				inbuf_ = new boost::iostreams::filtering_streambuf<boost::iostreams::input>();
				inbuf_->push(boost::iostreams::gzip_decompressor());
				inbuf_->push(inS_);
				iS_ = new std::istream(inbuf_);
				isCompressed_ = true;
			}
			else if (fileName.extension() == ".bz2")
			{
				inS_.open(fileName.string().c_str(), std::ios_base::in | std::ios_base::binary);
				inbuf_ = new boost::iostreams::filtering_streambuf<boost::iostreams::input>();
				inbuf_->push(boost::iostreams::bzip2_decompressor());
				inbuf_->push(inS_);
				iS_ = new std::istream(inbuf_);
				isCompressed_ = true;
			}
			else
			{
				inS_.open(fileName.string().c_str());
				iS_ = &inS_;
				isCompressed_ = false;
			}
		}
		// throw an exception if opening fails
		if (inS_.fail())
			throw std::ios_base::failure("Error opening file '" + fileName.string() + "': " +strerror(errno));
		active_=true;
	}


	Input&
	getline(Input& in, std::string &line)
	{
		if (!getline(*in.iS_, line))
			in.active_ = false;
		return in;
	}
}
