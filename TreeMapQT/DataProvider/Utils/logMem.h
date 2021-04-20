#pragma once
//==============================================================================
/// \copyright (c) 2015, DB-Matik GmbH, www.db-matik.de. All rights reserved.
//==============================================================================
/// \file log.h
/// \brief Provides a feature to log data during runtime to a file. 
//==============================================================================

//#include "FileSystem.h"

#include <string>
#include <ostream>
#include <iostream>
#include <streambuf>
#include <memory>
#include <functional>
//#include <boost/filesystem.hpp>

namespace global_namespace
{

#define LOGMEMTYPE std::shared_ptr<tLog<logPrefix::timestamp> >
#define	GETLOGMEMINST(name) tLog<logPrefix::timestamp>::instance(name)

#if WITH_LOGMEM
#define lerrDEF logOut->isEnabled(eLogLevel::error)   && std::cout << (*logOut) << "ERROR  " << boost::filesystem::path(__FILE__).stem().string() << ':' << __LINE__ << ' '
#define lerr	lerrDEF
#define lwarn	logOut->isEnabled(eLogLevel::warning) && (*logOut) << "WARN   " << boost::filesystem::path(__FILE__).stem().string() << ':' << __LINE__ << ' '
#define linfo	logOut->isEnabled(eLogLevel::info)    && (*logOut) << "INFO   " << boost::filesystem::path(__FILE__).stem().string() << ':' << __LINE__ << ' '
#define ldetail logOut->isEnabled(eLogLevel::detail)  && (*logOut) << "DETAIL " << boost::filesystem::path(__FILE__).stem().string() << ':' << __LINE__ << ' '
#define ldbg	logOut->isEnabled(eLogLevel::debug)   && (*logOut) << "DEBUG  " << boost::filesystem::path(__FILE__).stem().string() << ':' << __LINE__ << ' '
#define lraw	logOut->isEnabled()                   && CLogStream(logOut->ostream())
#define lout  logOut->isEnabled(eLogLevel::info)    && (*logOut) << "INFO   " << boost::filesystem::path(__FILE__).stem().string() << ':' << __LINE__ << ' '
#define XMLERROR (*logOut) << "XMLERROR   " << boost::filesystem::path(__FILE__).stem().string() << ':' << __LINE__ << ' '

#else
#define lerrDEF std::cout << std::endl << " ERROR  " << __FILE__ << ':' << __LINE__ << ' '
#define lerr	lerrDEF
#define lwarn	std::cout << std::endl << "WARN   " << __FILE__ << ':' << __LINE__ << ' '
#define linfo	std::cout //<< std::endl //<< "INFO   " //<< fs::filename(__FILE__) << ':' << __LINE__ << ' '
#define ldetail std::cout << std::endl << "DETAIL " << __FILE__ << ':' << __LINE__ << ' '
#define ldbg	std::cout << std::endl << "DEBUG  " << __FILE__ << ':' << __LINE__ << ' '
#define lraw	std::cout << std::endl << ' ' 

#define lout  std::cout << std::endl //<< "INFO   " << fs::filename(__FILE__) << ':' << __LINE__ << ' '

#define XMLERROR std::cout << "XMLERROR   " << __FILE__ << ':' << __LINE__ << ' '
#endif

		//==============================================================================
		/// \brief Definition of the support log stream levels
		enum class eLogLevel : unsigned long
		{
			debug = 0, detail, info, warning, error, none
		};

		//==============================================================================
		/// \brief Defines the kind of information to be send as a prefix with the logging informations
		enum class logPrefix
		{
			none,
			timestamp
		};

#if WITH_LOGMEM
		class CLogStream;

		//==============================================================================
		/// \brief Logs data during runtime to speciefed file
		template <logPrefix P>
		class tLog
		{
		private:
			/// \param name Name of the output stream. If name is empty this logger is disabled.
			tLog(const std::string& name = std::string());
		public:
			tLog(const tLog&);
			tLog(tLog&&);
			tLog& operator = (const tLog&);
			tLog& operator = (tLog&&);
			std::shared_ptr<tLog> static instance(const std::string& name = std::string())
			{
				return std::shared_ptr<tLog>(new tLog(name));
			};

			~tLog();

			void clearLogFile();

			template <typename T> CLogStream operator<<(const T& value);
			CLogStream operator<<(std::ostream& (*fp)(std::ostream&));

			operator bool() const;

			std::ostream& ostream()
			{
				return m_ostream;
			}

			bool isEnabled(eLogLevel level);
			bool isEnabled() const
			{
				return m_enabled;
			}

			std::string name() const
			{
				return m_shmName;
			}
		private:

			std::string m_shmName;
			std::shared_ptr<std::streambuf> m_streambuf;
			std::ostream m_ostream;
			bool m_enabled; ///< result of the last  bool isEnabled(eLogLevel level) call
		};

		using cLog = tLog<logPrefix::timestamp>;

		//==============================================================================
		/// \brief Prints the actual time in format YY MM DD hh:mm:ss:mmm.
		struct TimeStamp
		{
			TimeStamp() = default;
			std::ostream& operator << (std::ostream&) const;
		};

		std::ostream& operator << (std::ostream& os, const TimeStamp& timestamp);

		struct ILogStreamerLock;

		//==============================================================================
		/// \brief Makes sure that the logging stream will be locked until the whole package was transfered.
		class CLogStream
		{
			CLogStream(const CLogStream& ls) = delete;
			CLogStream& operator = (const CLogStream&) = delete;
			CLogStream& operator = (CLogStream&&) = delete;

		public:
			CLogStream(std::ostream& os);
			CLogStream(CLogStream&& ls);
			~CLogStream();

			template <typename T> CLogStream& operator<<(const T& value);
			CLogStream& operator<<(std::ostream& (*fp)(std::ostream&));

			operator bool() const
			{
				return true;
			}

		private:
			std::ostream& m_ostream;
			std::unique_ptr<ILogStreamerLock> m_lock;
		};

		template<> template <typename T> CLogStream tLog<logPrefix::none>::operator<<(const T& value)
		{
			CLogStream ls(m_ostream);
			ls << std::endl << value;
			return ls;
		}

		template <> template <typename T> CLogStream tLog<logPrefix::timestamp>::operator << (const T& value)
		{
			CLogStream ls(m_ostream);
			ls << std::endl << TimeStamp() << " " << value;
			return ls;
		}

		template <typename T> CLogStream& CLogStream::operator << (const T& value)
		{
			m_ostream << value;
			return *this;
		}
#else
		template <logPrefix P>
		class tLog
		{
		private:
			/// \param name Name of the output stream. If name is empty this logger is disabled.
			tLog(const std::string& name = std::string());
		public:
			std::shared_ptr<tLog> static instance(const std::string& name = std::string())
			{
				return std::shared_ptr<tLog>(new tLog(name));
            }

			~tLog();

			std::string name() const
			{
				return m_shmName;
			}
		private:

			std::string m_shmName;
		};



		using cLog = tLog<logPrefix::timestamp>;

#endif

}


