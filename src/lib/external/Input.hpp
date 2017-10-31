/*

 * Input.hpp
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


#ifndef INPUT_HPP_
#define INPUT_HPP_

// C++ header
#include <fstream>
#include <iostream>

// boost header
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/filesystem.hpp>

/**
 * \file Input.hpp
 * \brief This file contains the Input class.
 */


namespace AlgorithmPack
{

/**
 * \brief Class to handle files for reading.
 */
class Input
{

private:
	std::ifstream inS_;
	std::istream *iS_;
	boost::iostreams::filtering_streambuf<boost::iostreams::input> *inbuf_;
	bool isCompressed_;
	bool active_;
	bool isCin_;

public:

	/**
	 * \brief Standard constructor
	 */
	Input();


	/**
	 * \brief Constructor opening a file.
	 * @param fileName The filename to open.
	 */
	Input(const boost::filesystem::path &fileName);


	/**
	 * \brief Standard destructor
	 */
	virtual ~Input();


	/**
	 * \brief Opens a file.
	 *
	 * If the file is compressed with either gzip or bzip2 it will be decompressed. This will be activated by the
	 * file ending (gz/bz2).
	 * @param fileName The file to open.
	 * \exception std::ios_base::failure An exception giving information which file could not be opened.
	 */
	void
	open(const boost::filesystem::path &fileName);


	/**
	 * \brief Get the stream to read from.
	 * @return The stream
	 */
	std::istream&
	get()
	{
		return *iS_;
	};


	/**
	 * \brief moves to a different position in the file.
	 * Arguments are passed on to istream;
	 * @param args
	 */
	template<typename ...Args>
	void
	seekg(Args... args)
	{
		iS_->seekg(args...);
	}


	/**
	 * \brief Returns current position in the file
	 * @return The position.
	 */
	std::streampos
	tellg()
	{
		return iS_->tellg();
	}


	/**
	 * \brief Closes the file.
	 */
	void
	close()
	{
		if (!isCin_)
		if (&inS_ != iS_)
		{
			delete inbuf_;
			delete iS_;
		}
		inS_.close();

	};


	/**
	 * \brief Checks if the current file is a compressed file.
	 * @return true if the file is compressed, else false.
	 */
	bool
	isCompressed()
	{
		return isCompressed_;
	}


	friend Input&
	getline(Input& in, std::string &line);


	operator bool() const
	{
		return active_;
	}


};

/**
 * \brief Reads a line from a file.
 * @param in The input object.
 * @param line The string to store a line in.
 * @return The input object.
 */
Input&
getline(Input& in, std::string &line);

}



#endif /* INPUT_HPP_ */
