#pragma once

#include "XMLParameter.h"
//#include "XMLParameterValue.h"

#include "../Utils/Casting.h"
#include "../Utils/logMem.h"

#include <iostream>
#include <string> 
#include <map>
#include <boost/lexical_cast.hpp>

namespace global_namespace
{	

	template<typename T>
	T cXMLParameter::getDefault()
	{
		return m_Default.get<T>();
	}

	template<typename T>
	T cXMLParameter::getValueForKey(const std::string& key)
	{
		std::string lowerKey = Cast::lower(key);
		if (m_ValuesForKey.find(lowerKey) == m_ValuesForKey.end())
			return m_Default.get<T>();

		auto& it = m_ValuesForKey[lowerKey];
		if (it.size() > 0)
			return it[0].get<T>();

		return T{};
	}

	template<typename T>
	std::tuple<std::string, T> cXMLParameter::stuple(int idx)
	{
		auto it = m_ValuesForKey.begin();
		std::advance(it, idx);
		
		T s{};
		if (it->second.size() > 0)
			s = it->second[0].get<T>();

		return std::make_tuple(it->first, s);
	}

	template<typename keyT, typename valueT>
	std::tuple<keyT, valueT> cXMLParameter::tuple(int idx)
	{
		auto it = m_ValuesForKey.begin();
		std::advance(it, idx);
		std::string key = it->first;
		keyT t;
		try
		{
			t = boost::lexical_cast<keyT>(key);
		}
		catch (...)
		{
			lerr << "Lexical cast error of _data = " << key << ", to type: " << typeid(keyT).name() << std::endl;
			return std::make_tuple(keyT(), valueT());
		}

		valueT s{};
		if (it->second.size() > 0)
			s = it->second[0].get<valueT>();

		return std::make_tuple(t, s);
	}	

	template<typename... Args >
	bool unpack_int(std::vector<tUntypedValue>&v, size_t startIdx)
	{
		return false;
	}

	template<typename T>
	bool unpack_int(std::vector<tUntypedValue>&v, size_t startIdx, T& t)
	{
		if (startIdx >= v.size())
		{
			return false;
		}

		t = v[startIdx].get<T>();
		return true;
	}

	template<typename T, typename... Args >
	bool unpack_int(std::vector<tUntypedValue>&v, size_t startIdx, T& t, Args&... args)
	{
		if (startIdx >= v.size())
		{
			return false;
		}

		t = v[startIdx].get<T>();
		return unpack_int(v, ++startIdx, args...);
	}

	template<typename keyT, typename... Args >
	bool cXMLParameter::unpack(int idx, keyT& key, Args&... args)
	{
		auto it = m_ValuesForKey.begin();
		std::advance(it, idx);
		std::string keyS = it->first;		
		try
		{
			key = boost::lexical_cast<keyT>(keyS);
		}
		catch (...)
		{
			lerr << "Lexical cast error of _data = " << key << ", to type: " << typeid(keyT).name() << std::endl;
			return false;
		}

		return unpack_int(it->second, 0, args...);
		//return std::make_tuple(t, it->second.get<valueT>());
	}

}
