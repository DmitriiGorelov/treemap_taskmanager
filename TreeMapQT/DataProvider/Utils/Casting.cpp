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

bool Cast::is_float(const std::string& str)
{
	static float value(0.0);
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

bool Cast::is_double(const std::string& str)
{
	static double value(0.0);
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

double Cast::to_double(const std::string& str)
{
	try
	{
		return boost::lexical_cast<double>(str);
	}
	catch (...)
	{
		return 0.0;
	}
	return 0.0;
}

int Cast::to_int(const std::string& str)
{
	try
	{
		return boost::lexical_cast<int>(str);
	}
	catch (...)
	{
		return 0;
	}
	return 0;
}


std::string Cast::lower(const std::string& name)
{
	return boost::algorithm::to_lower_copy(name);
}