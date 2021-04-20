#pragma once

#include <boost/lexical_cast.hpp>

//#include "definitions.h"
#ifdef WIN32
	#include <time.h>
#else
	#include <sys/time.h>			// For time structure
#endif

#include <string>
#include <sstream>

// SEMAPHORES

#define SEM_RUN         0
#define SEM_READY       1
#define SEM_ERROR      -1

#define MeasureTime 0

#define CharToConstChar(x) std::string(x).c_str()

const std::string& path_delim();

//==============================================================================
    /// \brief Definition of the posible signal levels for digital IOs.
	namespace IOSignalLevel {
		enum T
		{
			activeHigh = 0,
			activeLow = 1
		};
	};
	
	typedef unsigned long long timerFlag_t;

	/*std::string to_str::get(double d);
	std::string to_str::get(int d);
	std::string llto_string(long long d);
	std::string ullto_string(unsigned long long d);*/

	//template<typename T>
	struct to_str
	{		
		/*template<typename T>
		to_str::get(T v)
			: _v(v)
		{

		}*/


		template <typename T>
		static std::string get(T _v)
		{			
			//sometimes triggers an error
			/*static std::string s("<TYPE CAST FAIL>");
			s = boost::lexical_cast<std::string>(T());
			try
			{
				s = boost::lexical_cast<std::string>(_v);
				//s=std::to_string(_v);
			}
			catch (...)
			{
				//s= "<TYPE CAST FAIL>";
			}
			return s;*/
			std::stringstream s;
			s << _v;
			return s.str();
		}		
	};

	bool TimeExpiredMsec(const timerFlag_t TimeBegin, const timerFlag_t Interval);
	timerFlag_t GetMsecTime();
	timerFlag_t GetUsecTime();
	std::string GetUsecTimeStr();
	double dabs(double v);

	void usleep(int usec);

	void DEBUG(bool value);
	bool DEBUG();

	void Terminate();
	bool Terminated();

	std::string appFolder();
	void appFolder(const std::string& value);

	std::string backupFolder();
	void backupFolder(const std::string& value);

	double Mind(double a, double b);
	double Maxd(double a, double b);
	
