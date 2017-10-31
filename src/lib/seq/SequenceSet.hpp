/*
 * SequenceSet.h
 *
 *  Created on: Sep 6, 2015
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

#ifndef SRC_SEQ_SEQUENCESET_HPP_
#define SRC_SEQ_SEQUENCESET_HPP_

#include <map>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "Sequence.hpp"
#include "../external/Input.hpp"
#include "../external/Output.hpp"
#include "../utility/utility.hpp"

#include <boost/filesystem.hpp>

/**
 * \file SequenceSet.hpp
 * \brief This file contains the SequenceSet class..
 */

namespace BBL
{

/**
 * \brief Simple class representing a set of sequences.
 */
class SequenceSet
{
private:
	std::vector<std::unique_ptr<Sequence> >  seqs_;

	void
	readFasta_(AlgorithmPack::Input &inS, std::map<std::string, int> &names, bool keep);

	void
	readFasta_(AlgorithmPack::Input &inS, const std::string &re, short place, bool keep);

	void
	readFasta_(AlgorithmPack::Input &inS, std::map<std::string, int> &names, const boost::filesystem::path &index, bool keep);

	void
	readFasta_(AlgorithmPack::Input &inS, const std::string &re, const boost::filesystem::path &index, bool keep);

public:
	/**
	 * \brief Standard constructor
	 */
	SequenceSet();
	SequenceSet(boost::filesystem::path &path);
	SequenceSet(SequenceSet&& seq) = default;
    SequenceSet& operator=(SequenceSet&& seq) = default;

	/**
	 * \brief Destructor
	 */
	virtual ~SequenceSet();




	/**
	 * \brief Accessing a sequence by index.
	 * @param index The index to use.
	 * @return reference to a sequence.
	 */
	Sequence
	&operator[](size_t index)
	{
		return *seqs_[index];
	}

	/**
	 * \overload
	 */
	const Sequence
	&operator[](size_t index) const
	{
		return *seqs_[index];
	}

	/**
	 * \brief Adds a sequence to the set.
	 * @param seq The sequence to add.
	 */
	void
	push_back(Sequence *seq)
	{
		seqs_.emplace_back(seq);
	}


	/**
	 * \brief Ads a sequence to the end.
	 * @param args The constructor arguments for a sequence.
	 */
	template <typename... Args>
	void
	emplace_back(Args&&... args)
	{
		seqs_.emplace_back(new Sequence(std::forward<Args>(args)...));
	}

	template <typename... Args>
	void
	erase(Args&&... args)
	{
		seqs_.erase(std::forward<Args>(args)...);
	}

	void
	read(const boost::filesystem::path &inFile, bool ignoreMissing=false)
	{
		read(inFile, std::set<std::string>(), true, "", ignoreMissing);
	}



	template <typename T>
	void
	read(const boost::filesystem::path &inFile, const T &names, const boost::filesystem::path &index, bool ignoreMissing=false)
	{
		read(inFile, names, true, index, ignoreMissing);
	}

	/**
	 * \brief Reads a sequence file.
	 * @param inFile The file to read.
	 * @param names A set of names if not all sequences should be read.
	 * @param index An index file to speed up extraction a sequence.
	 */
	template <typename T>
	void
	read(const boost::filesystem::path &inFile, const T &names, bool keep=true, const boost::filesystem::path &index="", bool ignoreMissing=false)
	{
		std::map<std::string, int> n;
		for (auto name : names)
			n.emplace(name, 0);
		AlgorithmPack::Input in(inFile);

		if (index.empty())
			readFasta_(in, n, keep);
		else
			readFasta_(in, n, index, keep);
		in.close();

		if ((!ignoreMissing) && ((!names.empty()) && (keep)))
		{
			for (auto &pair : n)
			{
				if (pair.second == 0)
					throw std::runtime_error("Sequence '" + pair.first + "' not found!");
				if (pair.second > 1)
					throw std::runtime_error("Sequence '" + pair.first + "' appears more than once!");
			}
		}
	}

	/**
	 * \brief Reads a sequence file.
	 * @param inFile The file to read.
	 * @param names A set of names if not all sequences should be read.
	 * @param index An index file to speed up extraction a sequence.
	 */
	void
	read(const boost::filesystem::path &inFile, const std::string &regex, short place, bool keep=true, const boost::filesystem::path &index="")
	{
		AlgorithmPack::Input in(inFile);

		if (index.empty())
			readFasta_(in, regex, place, keep);
		else
			readFasta_(in, regex, index, keep);
		in.close();
	}

	void
	write(const boost::filesystem::path &outfile, bool noComments=false, std::ios_base::openmode mode = std::ios_base::out);

	/**
	 * \brief Returns the number of sequences in the set.
	 * @return The number of sequences.
	 */
	size_t
	size() const noexcept
	{
		return seqs_.size();
	}

	void
	resize(size_t n) {
		for (size_t i = 0; i<n; ++i)
			this->push_back(new Sequence("", ""));
	}
	/**
	 * \brief Removes all sequences from the set.
	 */
	void
	clear() noexcept
	{
		seqs_.clear();
	}

	std::vector<std::unique_ptr<Sequence> >::iterator
	begin()
	{
		return seqs_.begin();
	}

	std::vector<std::unique_ptr<Sequence> >::iterator
	end()
	{
		return seqs_.end();
	}

	std::vector<std::unique_ptr<Sequence> >::const_iterator
	begin() const
	{
		return seqs_.begin();
	}

	std::vector<std::unique_ptr<Sequence> >::const_iterator
	end() const
	{
		return seqs_.end();
	}
};


/**
 * \brief Writes and index of a fasta file into a new file.
 * @param inFile A fasta file for which the index will be constructed.
 * @param outFile The file into which the index will be written.
 */
void
makeIndex(const boost::filesystem::path &inFile, const boost::filesystem::path &outFile);

} /* namespace BBL */

#endif /* SRC_SEQ_SEQUENCESET_HPP_ */
