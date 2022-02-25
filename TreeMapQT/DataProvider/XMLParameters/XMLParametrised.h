#pragma once

#include <iostream>
#include <string> 
#include <unordered_map>
#include <vector>
#include <boost/lexical_cast.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <XML/pugixml.h>

#include "XMLParameter.h"

#include "XMLParameterNodes.h"

namespace global_namespace
{
	namespace XML
	{
		class cXMLContainer;
	}

	class cXMLParametrised
		{
        public:
			static std::string ParameterNode() { return "parameter"; }
			static std::string ParameterAttrName() { return "name"; }
			static std::string ParameterAttrValue() { return "value"; }

			cXMLParametrised(const std::string& _name, bool _reg);
			virtual ~cXMLParametrised();

            boost::shared_ptr<cXMLParameter> AddParameter(const std::string& name, const std::string& val);

            boost::shared_ptr<cXMLParameter> GetParameterPtr(const std::string& name);
            void SetParameterPtr(const std::string& name, boost::shared_ptr<cXMLParameter> ptr);

			std::string GetParameterText(const std::string& name);

			template <typename T>
			bool ParameterExists(const std::string& name, T& val, T deflt, bool makeOutput = false);

			template <typename T>
			T GetParameter(const std::string& name, T deflt);

            template <typename T>
            T GetParameter(const std::string& name, const std::string& key, T deflt);

			template <typename T>
			void SetParameter(const std::string& name, T val);

			void SetParameter(const std::string& name, std::string val);

			std::vector<std::string> ParameterListNames();

            size_t GetParameterMapSize(const std::string& name, bool makeOutput = false);

            template<typename keyT, typename valueT>
            std::tuple<keyT, valueT> GetParameterMapValueByIndex(const std::string& name, int idx, bool makeOutput=false);

            template<typename keyT, typename... valueT>
            bool GetParameterMapVectorByIndex(const std::string& name, int idx, keyT& key, valueT&... values);

            std::string xmlName() const;

			virtual void ApplyDefaultParameters(){}

			void SetXML(pugi::xml_node& node);

            bool operator <(const cXMLParametrised& b)
            {
                return m_Name < b.m_Name;
            }

            bool operator<(cXMLParametrised* b) const
            {
                return m_Name < b->m_Name;
            }

            virtual std::list<boost::shared_ptr<cXMLParametrised>> getChildrenOsc() {return std::list<boost::shared_ptr<cXMLParametrised>>();}

		protected:
			virtual void ParameterChangedCallback(const std::string& name, const std::string& val);
        private:

			template <typename T>
			bool value(const std::string& name, T& result, const T& deflt, bool makeOutput = false);

            template <typename T>
            bool valueForKey(const std::string& name, const std::string& key, T& result, const T& deflt, bool makeOutput = false);

        private:
			std::string m_Name;
            std::unordered_map<std::string, boost::shared_ptr<cXMLParameter> > m_Value;
			bool m_Reg;
			boost::shared_ptr<XML::cXMLContainer> m_XMLContainer;
		};

		typedef boost::shared_ptr<cXMLParametrised> pXMLParametrised;		
}
