//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file exception.h
/// \brief Offers a catchable exception object what creates it's throw msg at initialization time.
//==============================================================================

#pragma once
//===== macro for info message =====
/// \brief Defines to standard string.
#define S(x) (std::string)(x)
/// \brief Casts to standard string.
#define TS(x) (std::to_string)(x)
/// \brief Creates an info string with currently processed file and line.
#define INFO (S("File: ") + __FILE__ + S(" Line: ") + TS(__LINE__))

#include <exception>
#include <string>
#include <sstream>
#include <iostream>


/// \brief Custom exception class derived from standard exception.
class CException : public std::exception
{
public:
	/// \brief Constructor of exception class.
	/// This fills its string stream with an info msg.
	/// @param msg The msg which is thrown.
	/// @param info The through macros created info msg of the currently processed file and line.
	CException(std::string msg, std::string info) : m_sStream{}
	{
		m_sStream << "Exception: " << msg << std::endl << info << std::endl << "Press return to continue." << std::endl;
	}
	/// \brief Destructor of exception class - noexcept.
	~CException() throw() {}

	/// \brief Prints the string stream - noexcept.
	void print() throw() 
	{ 
		std::cerr << m_sStream.str(); 
		std::cin.ignore();
	}

private:
	/// \brief The string stream which could be filled with throw messages.
	std::stringstream m_sStream;
};