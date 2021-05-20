//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// exception.h
/// Offers a catchable exception object what creates it's throw msg at initialization time.
//==============================================================================

#pragma once
#include <exception>
#include <string>

class CException : public std::exception
{
public:
	CException(std::string info) : m_infomsg{ info } {}
	~CException() throw () {}

	const char* msg() throw()
	{
		m_infomsg += "\n";
		return m_infomsg.c_str();
	}

private:
	std::string m_infomsg;
};