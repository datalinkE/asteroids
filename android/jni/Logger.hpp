/*
 * Logger.hpp
 *
 *  Created on: 22.04.2014
 *      Author: Enter
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <android/log.h>
#include <sstream>
#include <string>

namespace Util
{
	class Logger
	{
	public:
		Logger(int priority, const char* tag)
			:	m_priority(priority)
			,	m_tag(tag)
		{};

		template<class T>
		inline Logger& operator << (const T& value)
		{
			if(m_content.tellp())
			{
				m_content << " ";
			}

			m_content << value;
			return *this;
		}

		~Logger()
		{
			__android_log_write(m_priority, m_tag.c_str(), m_content.str().c_str() );
		}

	private:
		int m_priority;
		std::string m_tag;
		std::stringstream m_content;
	};
}

#define DLOG() Util::Logger(ANDROID_LOG_DEBUG, __func__)

#endif /* LOGGER_HPP_ */
