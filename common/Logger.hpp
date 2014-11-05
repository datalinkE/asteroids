#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "platform_log.h"

namespace Util
{
	class Logger
	{
	public:
		Logger(int priority, const char* tag)
			:	m_priority(priority)
			,	m_tag(tag)
			, 	m_shouldPrintSeparator(false)
		{};

		template<class T>
		inline Logger& operator << (const T& value)
		{
			if(m_shouldPrintSeparator)
			{
				m_content << " ";
			}

			m_content << value;

			m_shouldPrintSeparator = true;

			return *this;
		}

	    // this is the type of std::cout
	    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

	    // this is the function signature of std::endl
	    typedef CoutType& (*StandardEndLine)(CoutType&);

	    // define an operator<< to take in std::endl
	    Logger& operator<<(StandardEndLine manip)
	    {
	        // call the function, but we cannot return it's value
	        manip(m_content);

	        m_shouldPrintSeparator = false;

	        return *this;
	    }


		~Logger()
		{
			if(!m_content.tellp())
			{
				// platform logging could ignore empty text
				m_content << m_tag;
			}

			log_write(m_priority, m_tag.c_str(), m_content.str().c_str() );
		}

	private:
		int m_priority;
		std::string m_tag;
		std::stringstream m_content;
		bool m_shouldPrintSeparator;
	};
}

#define DLOG() Util::Logger(LOG_LEVEL_DEBUG, __func__)

#define CRASH(e) Util::Logger(LOG_LEVEL_DEBUG, #e); __builtin_trap()

#define ARG(variable) #variable << "=" << variable
