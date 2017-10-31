/*
 * Output.hpp
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

#ifndef SRC_OUTPUT_HPP_
#define SRC_OUTPUT_HPP_

// C++ header
#include <fstream>
#include <iostream>

// boost header
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/filesystem.hpp>

/**
 * \file Output.hpp
 * \brief This file contains the Output class.
 */

namespace AlgorithmPack
{



/**
 * \brief A class to write data to a file.
 *
 * This class can handle file compression as well as writing to standard output.
 */
class Output
{

private:
	std::ofstream outS_;
	std::ostream *oS_;
	boost::iostreams::filtering_streambuf<boost::iostreams::output> *outbuf_;

public:
    /**
     * \brief Standard constructor.
     */
	Output();

	/**
	 * \brief Constructor opening a file.
	 * @param fileName The filename to open.
	 */
	Output(const boost::filesystem::path &fileName, std::ios_base::openmode mode = std::ios_base::out);


    /**
     * \brief Destructor
     */
	virtual ~Output();

	/**
	 * \brief Open a file
	 * @param fileName The file to open.
	 * \exception std::ios_base::failure An exception giving information which file could not be opened.
	 */
	void
	open(const boost::filesystem::path &fileName, std::ios_base::openmode mode = std::ios_base::out);


	/**
	 * \brief Get the stream.
	 * @return The stream
	 */
	std::ostream&
	get()
	{
		return *oS_;
	};


	/**
	 * \brief Closes the file.
	 */
	void
	close()
	{
		if ((&outS_ != oS_) && (oS_ != &std::cout))
		{
			delete outbuf_;
			delete oS_;
		}
		outS_.close();
	};

	/**
	 * \brief Output operator
	 * @param os The output object.
	 * @param obj The object to print
	 * @return The output object
	 */
	template <typename T>
	friend Output& operator<<(Output& os, const T& obj);

};

template <typename T>
Output& operator<<(Output& os, const T& obj)
{
    *(os.oS_) << obj;
    return os;
}

}

#endif /* SRC_OUTPUT_HPP_ */


