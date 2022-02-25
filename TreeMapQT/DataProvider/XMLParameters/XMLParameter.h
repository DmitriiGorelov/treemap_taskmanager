#pragma once

#include "XMLParameterNodes.h"

#include <iostream>
#include <string> 
#include <unordered_map>
#include <vector>
#include <boost/lexical_cast.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <XML/pugixml.h>

#include "../Utils/UntypedValue/UntypedValue.h"

namespace global_namespace
{

	namespace {
		namespace eParameterReferenceType
		{
			typedef int T;
			enum E : T 
			{
				ThisParameter=0,				
				PlugParameter=1,
				GlobalParameter = 2,
			};
		}
	}
	class cXMLParameter
	{
	public:
		/*static std::string ParameterNode() { return "parameter"; }
		static std::string ParameterAttrName() { return "name"; }
		static std::string ParameterAttrValue() { return "value"; }*/
		
		cXMLParameter()
            : m_Default()
			, m_ValuesForKey()
		{

		}

        cXMLParameter(const std::string& _default)
            : m_Default(_default)
			, m_ValuesForKey()
		{

		}

		virtual ~cXMLParameter()
		{}

		template<typename T>
		T getDefault();

		void setDefault(const std::string& value);

		void addValueForKey(const std::string& key, const std::string& value);

		template<typename T>
        T getValueForKey(const std::string& key);

		size_t size() { return m_ValuesForKey.size(); }

		template<typename keyT, typename valueT>
		std::tuple<keyT, valueT> tuple(int idx);

		/*template<typename T, typename... Args >
		void unpack_int(T& t, Args&... args);*/

		template<typename keyT, typename... Args >
		bool unpack(int idx, keyT& key, Args&... args);

		template<typename T>
		std::tuple<std::string, T> stuple(int idx);

        void SetXML(pugi::xml_node& node, const std::string& key)
        {
            // parameter attributes
            auto para = node.append_child(XMLNodeParameter);
            para.append_attribute(XMLAttrName) = key.c_str();
            para.append_attribute(XMLAttrValue) = getDefault<std::string>().c_str();                        
            int i(0);
            for (auto& it : m_ValuesForKey)
            {
                i++;
                auto mode = para.append_child(XMLNodeParameterMode);

                mode.append_attribute(XMLAttrName) = it.first.c_str();

                for (auto& val : it.second)
                {                    
                    mode.append_attribute((XMLAttrValue+std::to_string(i)).c_str()) = val.get<std::string>().c_str();
                }
            }
        }

	private:
        tUntypedValue m_Default;

		std::unordered_map<std::string, std::vector<tUntypedValue> > m_ValuesForKey;
	};

	typedef boost::shared_ptr<cXMLParameter> pXMLParameter;
}
