#pragma once

#include <string>
#include <boost/lexical_cast.hpp>

class Cast
{
public:
	template <typename T>
	static bool is_int(const std::string& str, T& value)
	{
		try
		{
			value = boost::lexical_cast<T>(str);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}	

	template <typename T>
	static bool is_float(const std::string& str, T& value)
	{
		try
		{
			value = boost::lexical_cast<T>(str);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	/*template <typename E>
	static std::string to_str(const E& value)
	{
		try
		{
			return boost::lexical_cast<std::string>(value);
		}
		catch (...)
		{
			return "0";
		}
		return "0";
	}*/

	static bool is_float(const std::string& str);
	static bool is_double(const std::string& str);

	static double to_double(const std::string& str);
	static int to_int(const std::string& str);

	static std::string lower(const std::string& name);

	/*template<typename E>
	static E to(const std::string& str)
	{
		return boost::lexical_cast<E>(str);
	}*/

	template<typename T>
	static T to(const std::string& str)
	{
		try
		{
			return boost::lexical_cast<T>(str);
		}
		catch (...)
		{
			return T(0);
		}
		return T(0);
	}
	
	template <typename T>
	static std::string to_str(T _v) 
	{
		// !!! this function returns symbol \0 if input value is int 0
		/*std::stringstream s;
		s << _v;
		return s.str();*/
		try
		{
			return boost::lexical_cast<std::string>(_v);
		}
		catch (...)
		{
			return "";
		}
		return "";
	}
};