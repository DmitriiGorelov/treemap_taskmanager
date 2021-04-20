#pragma once

#include "XMLParametrised.h"
#include "../Utils/Casting.h"

#include <iostream>
#include <string> 
#include <map>
#include <boost/lexical_cast.hpp>

namespace global_namespace
{

	template <typename T>
	bool cXMLParametrised::ParameterExists(const std::string& name, T& val, T deflt, bool makeOutput)
	{
		return value<T>(name, val, deflt, makeOutput);
	}

	template <typename T>
	T cXMLParametrised::GetParameter(const std::string& name, T deflt)
	{
		T val;
		value<T>(name, val, deflt);
		return val;
	}

	template <typename T>
	void cXMLParametrised::SetParameter(const std::string& name, T val)
	{
		AddParameter(name, boost::lexical_cast<std::string>(val));
	}

	template <typename T>
	bool cXMLParametrised::value(const std::string& name, T& result, const T& deflt, bool makeOutput)
	{
		result = deflt;

		if (name.empty() || m_Value.find(lower(name)) == m_Value.end())
		{
			if (makeOutput)
			{
				std::cerr << "Parameter " << lower(name) << " not found" << std::endl;
			}
			return false;
		}

		std::string value(m_Value[lower(name)]);

		if (value.empty())
		{
			return false;
		}
		value = CheckReference(value);

		try
		{						
			result = boost::lexical_cast<T>(value);
			
			return true;
		}
		catch (...)
		{
			std::cerr << "Lexical cast error of " << name << ", value = " << value << std::endl;
			return false;
		}
		return false;
	}
}
