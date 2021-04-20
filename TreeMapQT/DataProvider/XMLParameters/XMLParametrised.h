#pragma once

#include <iostream>
#include <string> 
#include <unordered_map>
#include <vector>
#include <list>
#include <boost/lexical_cast.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <XML/pugixml.h>

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

			void AddParameter(const std::string& name, const std::string& val);

			std::string GetParameterText(const std::string& name);

			template <typename T>
			bool ParameterExists(const std::string& name, T& val, T deflt, bool makeOutput = false);

			template <typename T>
			T GetParameter(const std::string& name, T deflt);

			template <typename T>
			void SetParameter(const std::string& name, T val);

			void SetParameter(const std::string& name, std::string val);

			std::vector<std::string> ParameterListNames();

			std::string xmlName();

			virtual void ApplyDefaultParameters(){}

			void SetXML(pugi::xml_node& node);

            virtual std::list<boost::shared_ptr<cXMLParametrised>> getChildrenOsc() {return std::list<boost::shared_ptr<cXMLParametrised>>();}

		protected:
			virtual void ParameterChangedCallback(const std::string& name, const std::string& val);
		private:
			/*bool value(const std::string& name, std::string& result, const std::string& deflt, bool makeOutput = false)
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

				try
				{
					std::string value(m_Value[lower(name)]);

					if (value.empty())
					{
						return false;
					}
					result = value;
					return true;
				}
				catch (...)
				{
					std::cerr << "Lexical cast error of " << name << std::endl;
					return false;
				}
				return false;
			}*/

            std::string CheckReference(const std::string& value);

			template <typename T>
			bool value(const std::string& name, T& result, const T& deflt, bool makeOutput = false);

		private:
			std::string m_Name;
			std::unordered_map<std::string, std::string> m_Value;
			bool m_Reg;
			boost::shared_ptr<XML::cXMLContainer> m_XMLContainer;
		};

		typedef boost::shared_ptr<cXMLParametrised> pXMLParametrised;		
}
