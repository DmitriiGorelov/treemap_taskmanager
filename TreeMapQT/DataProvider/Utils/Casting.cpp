#include "Casting.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

bool is_int(const std::string& str)
{
	try
	{
		int value = boost::lexical_cast<int>(str);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool is_float(const std::string& str)
{
	try
	{
		float value = boost::lexical_cast<float>(str);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool is_double(const std::string& str)
{
	try
	{
		double value = boost::lexical_cast<double>(str);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool is_int(const std::string& str, int& value)
{
	try
	{
		value = boost::lexical_cast<int>(str);		
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool is_float(const std::string& str, float& value)
{
	try
	{
		value = boost::lexical_cast<float>(str);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool is_double(const std::string& str, double& value)
{
	try
	{
		value = boost::lexical_cast<double>(str);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

double to_double(const std::string& str)
{
	static double result(0.0);
	try
	{
		result = boost::lexical_cast<double>(str);
	}
	catch (...)
	{
		result = 0.0;
	}
	return result;
}

int to_int(const std::string& str)
{
	static int result(0.0);
	try
	{
		result = boost::lexical_cast<int>(str);
	}
	catch (...)
	{
		result = 0;
	}
	return result;
}


std::string lower(const std::string& name) 
{
	return boost::algorithm::to_lower_copy(name);
}
