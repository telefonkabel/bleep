//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// exception.h
/// Offers a catchable exception object what creates it's throw msg at initialization time.
//==============================================================================

#pragma once
//macro for info message
#define INFO ((std::string)("\nFile: ") + __FILE__ + (std::string)("\nLine: ") + std::to_string(__LINE__) + (std::string)("\n"))

#include <exception>
#include <string>

class CException : public std::exception
{
public:
	CException(std::string msg, std::string info) : m_infomsg{} 
	{
		m_infomsg = "Exception: " + msg + info;
	}
	~CException() throw () {}

	const char* msg() throw() {	return m_infomsg.c_str(); }

private:
	std::string m_infomsg;
};