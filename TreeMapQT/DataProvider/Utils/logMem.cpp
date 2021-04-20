//==============================================================================
/// \copyright (c) 2015, DB-Matik GmbH, www.db-matik.de. All rights reserved.
//==============================================================================
/// \file log.cpp
/// \brief Provides a feature to log data during runtime. 
//==============================================================================

#include "logMem.h"

//#include "ShmStreamBuf.h"
//#include "XMLFileStreamer.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <mutex>
#include <map>


namespace global_namespace
{

#if WITH_LOGMEM
	//==============================================================================
	class CLoggingParam
	{
	public:
		CLoggingParam(const std::string& streamName);
		~CLoggingParam();
		operator std::string() const
		{
			return m_streamName;
		}
		operator unsigned long() const
		{
			return static_cast<unsigned long>(m_logLevel);
		}
		operator eLogLevel() const
		{
			return m_logLevel;
		}

	private:
		enum
		{
			String = 0, Level
		};
		std::map<std::string, eLogLevel>& conversionMap() const;
		std::string toStr(eLogLevel level) const;
		eLogLevel toLevel(const std::string& level) const;
		eLogLevel dfltLevel() const;

		std::string m_streamName;
		eLogLevel m_logLevel;
	};

	CLoggingParam::CLoggingParam(const std::string& stream)
		: m_streamName(stream)
		, m_logLevel(dfltLevel())
	{
		bool isItemMissing(false);
		const std::string fileName = []()
		{
			if (fs::exists("./../settings"))
				return "./../settings/machine/loggingStreams.xml";
			else
				return "./settings/machine/loggingStreams.xml";
		} ();
		const std::string streamName = m_streamName + ".stream-name";
		const std::string logLevel = m_streamName + ".log-level";
		std::set<database::dbEntry, database::lessDbEntry> entries;
		try
		{
			xml::CDbEntryFile file;
			entries = file.read(fileName);
		}
		catch (const std::string&)
		{
			isItemMissing = true;
		}

		{
			xml::CDbEntryFile::tSet::iterator it(entries.find(database::makeEntry(streamName, "")));
			if (it == entries.end())
			{
				it = entries.insert(database::makeEntry(streamName, m_streamName)).first;
				isItemMissing = true;
			}
			m_streamName = database::get<database::Value>(it);
		}
		{
			xml::CDbEntryFile::tSet::iterator it(entries.find(database::makeEntry(logLevel, "")));
			if (it == entries.end())
			{
				it = entries.insert(database::makeEntry(logLevel, toStr(dfltLevel()))).first;
				isItemMissing = true;
			}
			m_logLevel = toLevel(database::get<database::Value>(it));
		}

		if (isItemMissing)
		{
			try
			{
				xml::CDbEntryFile file;
				file.write(fileName, entries);
			}
			catch (const std::string&)
			{
				isItemMissing = true;
			}
		}
	}

	CLoggingParam::~CLoggingParam()
	{
	}

	std::map<std::string, eLogLevel>& CLoggingParam::conversionMap() const
	{
		static std::map<std::string, eLogLevel> map = []()
		{
			std::map<std::string, eLogLevel> map;
			map["debug"] = eLogLevel::debug;
			map["detail"] = eLogLevel::detail;
			map["info"] = eLogLevel::info;
			map["warning"] = eLogLevel::warning;
			map["error"] = eLogLevel::error;
			map["none"] = eLogLevel::none;
			return map;
		} ();
		return map;
	}

	eLogLevel CLoggingParam::dfltLevel() const
	{
#ifdef _DEBUG
		return eLogLevel::detail;
#else
		return eLogLevel::info;
#endif
	}

	std::string CLoggingParam::toStr(eLogLevel level) const
	{
		for (auto& entry : conversionMap())
		{
			if (std::get<Level>(entry) == level)
				return std::get<String>(entry);
		}
		return toStr(dfltLevel());
	}

	eLogLevel CLoggingParam::toLevel(const std::string& level) const
	{
		std::map<std::string, eLogLevel> map(conversionMap());
		auto it = map.find(level);
		if (it != map.end())
			return it->second;
		else
			return dfltLevel();
	}

	//==============================================================================
	std::ostream& TimeStamp::operator << (std::ostream& os) const
	{
		std::chrono::high_resolution_clock::time_point p(std::chrono::high_resolution_clock::now());
		std::chrono::microseconds msec(std::chrono::duration_cast<std::chrono::microseconds>(p.time_since_epoch()));
		std::chrono::seconds sec(std::chrono::duration_cast<std::chrono::seconds>(msec));

		std::time_t t = sec.count();
		std::size_t fractionalSeconds = msec.count() % 1000000;

		/// note: localtime() is not threadsafe, lock with a mutex is necessary, but around stream there is allready a lock
		std::vector<char> buffer(25); // holds "2013-12-01 21:31:42"
		strftime(buffer.data(), buffer.size(), "%Y-%m-%d %H:%M:%S:", localtime(&t));
		return os << buffer.data() << std::setw(6) << std::setfill('0') << fractionalSeconds;
	}

	std::ostream& operator << (std::ostream& os, const TimeStamp& ts)
	{
		return ts << os;
	}

	//==============================================================================

	struct NullBuffer : public std::streambuf
	{
		int overflow(int c)
		{
			return c;
		}
	};

	///< Wrapper around the stream logging mechanism
	struct ILogStreamerLock
	{
		virtual ~ILogStreamerLock()
		{
		}
	};

	//==============================================================================
	/// \brief Wrapper around boost stuff.
	class cLogStreamerLock : public ILogStreamerLock
	{
		cLogStreamerLock(const cLogStreamerLock&) = delete;
		cLogStreamerLock& operator = (const cLogStreamerLock&) = delete;
		cLogStreamerLock(cLogStreamerLock&&) = delete;
		cLogStreamerLock& operator = (cLogStreamerLock&&) = delete;

	public:
		cLogStreamerLock(boost::interprocess::interprocess_mutex& /*mtx*/, boost::interprocess::interprocess_condition& condition)
			: m_condition(condition)
		{
		}
		~cLogStreamerLock() final
		{
			if (&m_condition > 0)
				m_condition.notify_all();
		}

	private:
		boost::interprocess::interprocess_condition& m_condition;
	};

	//==============================================================================
	CLogStream::CLogStream(CLogStream&& ls) : m_ostream(ls.m_ostream), m_lock(std::move(ls.m_lock))
	{
	}
	CLogStream::~CLogStream()
	{
	}
	CLogStream::CLogStream(std::ostream& os)
		: m_ostream(os)
		, m_lock()
	{
		auto p = dynamic_cast<shmem::CShmStreamBuf*>(os.rdbuf());
		if (p)
			m_lock = std::unique_ptr<ILogStreamerLock>(new cLogStreamerLock(p->mutex(), p->condition()));
		else
			m_lock = std::unique_ptr<ILogStreamerLock>(new ILogStreamerLock());
	}

	CLogStream& CLogStream::operator<<(std::ostream& (*fp)(std::ostream&))
	{
		m_ostream << fp;
		return *this;
	}

	//==============================================================================
	template <logPrefix P> tLog<P>::tLog(const tLog& log)
		: m_shmName(log.m_shmName)
		, m_streambuf(log.m_streambuf)
		, m_ostream(m_streambuf.get())
		, m_enabled(log.m_enabled)
	{
	}

	template <logPrefix P> tLog<P>::tLog(tLog&& log)
		: m_shmName(std::move(log.m_shmName))
		, m_streambuf(std::move(log.m_streambuf))
		, m_ostream(m_streambuf.get())
		, m_enabled(std::move(log.m_enabled))
	{
	}

	template <logPrefix P> tLog<P>& tLog<P>::operator = (const tLog& log)
	{
		m_shmName = log.m_shmName;
		m_streambuf = log.m_streambuf;
		m_ostream.rdbuf(m_streambuf.get());
		m_enabled = log.m_enabled;
		return *this;
	}

	template <logPrefix P> tLog<P>& tLog<P>::operator = (tLog&& log)
	{
		m_shmName = std::move(log.m_shmName);
		m_streambuf = std::move(log.m_streambuf);
		m_ostream.rdbuf(m_streambuf.get());
		m_enabled = std::move(log.m_enabled);
		return *this;
	}

	template <logPrefix P> tLog<P>::tLog(const std::string& name)
		: m_shmName(name)
		, m_streambuf(std::make_shared<NullBuffer>())
		, m_ostream(m_streambuf.get())
		, m_enabled(false)
	{
		if (!m_shmName.empty())
		{
			CLoggingParam loggingParam(m_shmName);
			m_shmName = loggingParam.operator std::string();
			std::shared_ptr<shmem::CShmStreamBuf> streambuf(std::make_shared<shmem::CShmStreamBuf>(m_shmName));
			streambuf->setUserFlag(loggingParam);
			m_streambuf = streambuf;
			m_ostream.rdbuf(m_streambuf.get());
			m_ostream << std::endl << std::endl << "================ Open/Create LogFile " << m_shmName;
		}
	}

	template <logPrefix P> tLog<P>::~tLog()
	{
		if (m_streambuf)
			m_ostream << std::endl << std::endl << "================ Close LogFile " << m_shmName;
	}

	template <logPrefix P> void tLog<P>::clearLogFile()
	{
		auto ptr = dynamic_cast<shmem::CShmStreamBuf*>(m_streambuf.get());
		if (ptr) ptr->clear();
	}

	template <> CLogStream tLog<logPrefix::none>::operator<<(std::ostream& (*fp)(std::ostream&))
	{
		CLogStream ls(m_ostream);
		ls << std::endl << fp;
		return ls;
	}

	template <> CLogStream tLog<logPrefix::timestamp>::operator<<(std::ostream& (*fp)(std::ostream&))
	{
		CLogStream ls(m_ostream);
		ls << std::endl << TimeStamp() << " " << fp;
		return ls;
	}

	template <logPrefix P> tLog<P>::operator bool() const
	{
		return m_streambuf.operator bool();
	}

	template <logPrefix P> bool tLog<P>::isEnabled(eLogLevel level)
	{
		shmem::CShmStreamBuf* ptr(dynamic_cast<shmem::CShmStreamBuf*>(m_streambuf.get()));
		m_enabled = (ptr && static_cast<unsigned long>(level) >= ptr->getUserFlag());
		return m_enabled;
	}
#else
	template <logPrefix P> tLog<P>::tLog(const std::string& name)
		: m_shmName(name)
	{

	}

	template <logPrefix P> tLog<P>::~tLog()
	{

	}

#endif
	template class tLog<logPrefix::none>;
	template class tLog<logPrefix::timestamp>;
	
} // end of namespace global_namespace
