/*
 * Sequence.h
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

#ifndef SRC_SEQ_SEQUENCE_HPP_
#define SRC_SEQ_SEQUENCE_HPP_

// C++ header
#include <string>
#include <utility>

namespace BBL
{

/**
 * \file Sequence.hpp
 * \brief This file contains the Sequence class.
 */

/**
 * \brief A simple Sequence representation.
 */
class Sequence
{
private:
	std::string name_;
	std::string seq_;
	std::string comment_;

public:

    /**
     * \brief Standard constructor.
     */
	Sequence();

	/**
	 * \brief Constructor setting all values.
	 * \param name The name.
	 * \param seq The sequence.
	 * \param comment The comment.
	 */
	Sequence(const std::string &name, const std::string &seq, const std::string &comment="");
	Sequence(const std::string &name, std::string &&seq="", const std::string &comment="");

	Sequence(Sequence&& seq) = default;
	Sequence& operator=(Sequence&& seq) = default;
	Sequence(Sequence& seq) = default;
	Sequence& operator=(Sequence& seq) = default;


	virtual ~Sequence();

	/**
	 * \brief Access operator.
	 * \param index The index of to access.
	 */
	char
	&operator[](size_t index)
	{
		return seq_[index];
	}

	/**
	 * \overload
	 */
	const char
	&operator[](size_t index) const
	{
		return seq_[index];
	}

    /**
     * \brief Appends a sequnence segment to the existing sequence.
     */
	void
	append(std::string seq)
	{
		seq_.append(seq);
	}

	void
	push_back(char c)
	{
		seq_.push_back(c);
	}

    /**
     * \brief Returns the sequence.
     * \return The sequence.
     */
	std::string
	seq() const
	{
		return seq_;
	}

	void
	clear()
	{
		seq_.clear();
	}

    /**
     * \brief Returns the name of the sequence.
     * \return The name.
     */
	std::string
	name() const
	{
		return name_;
	}


	void
	name(const std::string &n)
	{
		name_= n;
	}

    /**
     * \brief Returns the sequence comment.
     * \return the comment.
     */
	std::string
	comment() const
	{
		return comment_;
	}

	void
	comment(const std::string &c)
	{
		comment_ = c;
	}

	/**
	 * \brief The length of the sequence.
	 * @return The length.
	 */
	size_t
	size() const noexcept
	{
		return seq_.size();
	}

	void
	resize(size_t length)
	{
		seq_.resize(length);
	}

	void
	reserve(size_t length)
	{
		seq_.reserve(length);
	}


	typedef typename std::string::iterator iterator;
	typedef typename std::string::const_iterator const_iterator;
	typedef typename std::string::reverse_iterator reverse_iterator;
	typedef typename std::string::const_reverse_iterator const_reverse_iterator;



 	/**
 	 * \brief Returns an iterator to the first element in the container.
 	 * @return An iterator to the first element in the container.
 	 */
	iterator begin() throw()
	{
		return seq_.begin();
	}

	/**
	 * \brief Returns an iterator to the past-the-end element in the container.
	 * @return An iterator to the past-the-end element.
	 */
	iterator end() throw()
	{
		return seq_.end();
	}

 	/**
 	 * \brief Returns a const_iterator to the first element in the container.
 	 * @return An iterator to the first element in the container.
 	 */
	const_iterator begin() const throw()
	{
		return seq_.begin();
	}


	/**
	 * \brief Returns a const_iterator to the past-the-end element in the container.
	 * @return An iterator to the past-the-end element.
	 */
	const_iterator end() const throw()
	{
		return seq_.end();
	}

	/**
	 * \brief Returns a reverse_iterator to the last element in the BinarySearchTree.
	 * @return An iterator to the last element.
	 */
	reverse_iterator rbegin() throw()
	{
		return seq_.rbegin();
	}

	/**
	 * \brief Returns a const_reverse_iterator to the past-the-end element in the container.
	 * @return An iterator to the past-the-end element.
	 */
	reverse_iterator rend() throw()
	{
		return seq_.rend();
	}

 	/**
 	 * \brief Returns a const_iterator to the first element in the container.
 	 * @return An iterator to the first element in the container.
 	 */
	const_iterator cbegin() const throw()
	{
		return seq_.cbegin();
	}


	/**
	 * \brief Returns a const_iterator to the past-the-end element in the container.
	 * @return An iterator to the past-the-end element.
	 */
	const_iterator cend() const throw()
	{
		return seq_.cend();
	}

 	/**
 	 * \brief Returns a const_iterator to the first element in the container.
 	 * @return An iterator to the first element in the container.
 	 */
	const_reverse_iterator crbegin() throw()
	{
		return seq_.crbegin();
	}

	/**
	 * \brief Returns a const_iterator to the past-the-end element in the container.
	 * @return An iterator to the past-the-end element.
	 */
	const_reverse_iterator crend() const throw()
	{
		return seq_.crend();
	}



};

} /* namespace BBL */

#endif /* SRC_SEQ_SEQUENCE_HPP_ */
