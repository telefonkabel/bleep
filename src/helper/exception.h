//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file exception.h
/// \brief Offers a catchable exception object what creates it's throw msg at initialization time.
//==============================================================================

#pragma once
#include <exception>
#include <string>

class CException : public std::exception
{
public:
	CException(std::string info) : infomsg(info) {}
	~CException() throw () {}

	const char* msg() const throw() { return infomsg.c_str(); }

private:
	std::string infomsg;
};