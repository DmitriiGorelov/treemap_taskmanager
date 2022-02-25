#pragma once

#include "XMLParametrised.h"
#include "../Utils/Casting.h"

#include <iostream>
#include <string> 
#include <map>
#include <boost/lexical_cast.hpp>

#include "XMLParameter_Impl.h"

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
        value<T>(name, val, deflt, false);
		return val;
	}

    template <typename T>
    T cXMLParametrised::GetParameter(const std::string& name, const std::string& key, T deflt)
    {
        T val;
        valueForKey<T>(name, key, val, deflt, true);
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
        std::string lowername(Cast::lower(name));
        if (name.empty() || m_Value.find(lowername) == m_Value.end())
        {
            if (makeOutput)
            {
                std::cout << "Parameter " << lowername << " not found" << std::endl;
            }
            return false;
        }
        return false;
    }

    template <typename T>
    bool cXMLParametrised::valueForKey(const std::string& name, const std::string& key, T& result, const T& deflt, bool makeOutput)
    {
        result = deflt;
        std::string lowername(Cast::lower(name));
        if (name.empty() || m_Value.find(lowername) == m_Value.end())
        {
            if (makeOutput)
            {
                std::cout << "Parameter " << lowername << " not found" << std::endl;
            }
            return false;
        }

        auto& it = m_Value[lowername];
        /*if (it->ref()) // key value cannot be a ref
        {
            result = IFX::FXContainer::singleton()->GetParameter<T>(it->getValueForKey<std::string>(key), deflt);
            return true;
        }*/

        result = it->getValueForKey<T>(key);
    }

    template<typename keyT, typename valueT>
    std::tuple<keyT, valueT> cXMLParametrised::GetParameterMapValueByIndex(const std::string& name, int idx, bool makeOutput)
    {
        std::string lowername(Cast::lower(name));
        if (name.empty() || m_Value.find(lowername) == m_Value.end())
        {
            if (makeOutput)
            {
                std::cout << "Parameter " << lowername << " not found" << std::endl;
            }
            keyT k{};
            valueT v{};
            return std::make_tuple(k, v);
        }

        return m_Value[lowername]->tuple<keyT, valueT>(idx);
    }

    template<typename keyT, typename... valueT>
    bool cXMLParametrised::GetParameterMapVectorByIndex(const std::string& name, int idx, keyT& key, valueT&... values)
    {
        std::string lowername(Cast::lower(name));
        if (name.empty() || m_Value.find(lowername) == m_Value.end())
        {
            //if (makeOutput)
            {
                std::cout << "Parameter " << lowername << " not found" << std::endl;
            }
            return false;
        }

        return m_Value[lowername]->unpack<keyT, valueT...>(idx, key, values...);
    }
}
