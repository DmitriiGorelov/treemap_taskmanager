
#include "include.h"
#include <iostream>

#if WITH_BOOST_CHRONO
#include <boost/chrono.hpp>
#endif

//#include <boost/thread/thread.hpp>
#include <chrono>
#include <stdio.h>

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/time.h>
#endif

bool l_DEBUG=false;

bool _giTerminate = false;

std::string _appFolder = "";
std::string _backupFolder = "";
#if WITH_BOOST_FILESYSTEM
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#endif
#include <boost/compute/detail/getenv.hpp>
// Path delimiter symbol for the current OS.
const std::string& path_delim()
{
#if WITH_BOOST_FILESYSTEM
	static const std::string delim =
		boost::filesystem::path("/").make_preferred().string();
#else
    static const std::string delim = "/";
#endif
	return delim;
}

	bool TimeExpiredMsec(const timerFlag_t TimeBegin, const timerFlag_t Interval)
	{
		timerFlag_t CurrentTime(GetMsecTime());
		if ((CurrentTime-TimeBegin>=Interval) || (CurrentTime<0))
		{
			return true;
		}
		else
			return false;
	}

	timerFlag_t GetMsecTime()
	{		
		//static std::chrono::high_resolution_clock::time_point _begin(std::chrono::high_resolution_clock::now());		
		//return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _begin).count();		

		// absolute time we need for ForceProfiler
		return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}

	timerFlag_t GetUsecTime()
	{		
		//static std::chrono::high_resolution_clock::time_point _begin(std::chrono::high_resolution_clock::now());
		//return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _begin).count();		

		// absolute time we need for ForceProfiler
		return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}

	std::string GetUsecTimeStr()
	{
		timerFlag_t t=GetUsecTime();

		char buf[100];
		sprintf(buf,"%llu.%llu",t/1000,t%1000);

		return buf;
	}

	void usleep(int usec)
	{
        //boost::this_thread::sleep_for(boost::chrono::microseconds(usec));
	}

	double dabs(double v)
	{
		if (v<0)
			return v*(-1);
		else
			return v;
	}

	void DEBUG(bool value)
	{
		l_DEBUG=value;
	}

	bool DEBUG()
	{
		return l_DEBUG;
	}

	void Terminate()
	{
		_giTerminate=true;
	}

	bool Terminated()
	{
		return _giTerminate;
	}

	std::string appFolder()
	{
		return _appFolder;
	}

	void appFolder(const std::string& value) 
	{
		_appFolder = value;
	}

	std::string backupFolder()
	{
		return _backupFolder;
	}

	void backupFolder(const std::string& value)
	{
		_backupFolder = value;
	}

	double Mind(double a, double b)
	{
		if (a > b)
			return b;
		else 
			return a;
	}

	double Maxd(double a, double b)
	{
		if (a < b)
			return b;
		else
			return a;
	}
