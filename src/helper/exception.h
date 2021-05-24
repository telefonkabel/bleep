//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// exception.h
/// Offers a catchable exception object what creates it's throw msg at initialization time.
//==============================================================================

#pragma once
//macro for info message
#define S(x) (std::string)(x)
#define TS(x) (std::to_string)(x)
#define INFO (S("File: ") + __FILE__ + S(" Line: ") + TS(__LINE__))

#include <exception>
#include <string>
#include <sstream>
#include <iostream>


class CException : public std::exception
{
public:
	CException(std::string msg, std::string info) : m_sStream{}
	{
		m_sStream << "Exception: " << msg << std::endl << info << std::endl << "Press return to continue." << std::endl;
	}
	~CException() throw () {}

	void print() throw() 
	{ 
		std::cerr << m_sStream.str(); 
		std::cin.ignore();
	}

private:
	std::stringstream m_sStream;
};