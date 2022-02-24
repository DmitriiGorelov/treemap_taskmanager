
#include "XMLParametrised_Internals.h"

#include "XMLContainer.h"

#include "../Utils/Casting.h"

namespace global_namespace
{
		/*std::string cXMLParametrised::ParameterNode("parameter");
		std::string cXMLParametrised::ParameterAttrName("name");
		std::string cXMLParametrised::ParameterAttrValue("value");*/

		cXMLParametrised::cXMLParametrised(const std::string& _name, bool _reg)
			: m_Name(_name)
			, m_Value()
			, m_Reg(_reg)
			, m_XMLContainer(XML::XMLContainer::singleton())
		{
			if (_reg)
			{
				if (m_XMLContainer)
					m_XMLContainer->Register(this);
			}
		}

		cXMLParametrised::~cXMLParametrised()
		{
			if (m_Reg)
			{
				if (m_XMLContainer)
					m_XMLContainer->Unregister(this);
            }
        }

		std::string cXMLParametrised::GetParameterText(const std::string& name) 
		{
			if (name.empty() || m_Value.find(lower(name)) == m_Value.end())
			{				
				return "";
			}						

			return m_Value[lower(name)];
		}

		void cXMLParametrised::AddParameter(const std::string& name, const std::string& val)
		{
			if (name.empty())
			{
				return;
			}			

			m_Value[lower(name)] = val;

			ParameterChangedCallback(lower(name), val);
		}

		void cXMLParametrised::SetParameter(const std::string& name, std::string val)
		{
			AddParameter(name, val);
		}

		void cXMLParametrised::ParameterChangedCallback(const std::string& name, const std::string& val)
		{

		}

		std::vector<std::string> cXMLParametrised::ParameterListNames()
		{
			std::vector<std::string> res;
			for (auto it: m_Value)
			{
				res.push_back(it.first);
			}

			return res;
		}

		std::string cXMLParametrised::xmlName()
		{
			return m_Name;
		}

        static std::string refPrefix = "ref.";

        std::string cXMLParametrised::CheckReference(const std::string& value)
		{
			std::size_t pos = value.find(refPrefix.c_str());
			if (pos == 0)
            {
                throw "ref. not supported in cXMLParametrised";
                //return IFX::IFXContainer::singleton()->GetParameterText(value.substr(refPrefix.length()));
			}
			
			return value;
        }

		void cXMLParametrised::SetXML(pugi::xml_node& node)
		{            
			for (auto& it : cXMLParametrised::ParameterListNames())
			{
				auto para = node.append_child(cXMLParametrised::ParameterNode().c_str());
				para.append_attribute(cXMLParametrised::ParameterAttrName().c_str()) = it.c_str();
				para.append_attribute(cXMLParametrised::ParameterAttrValue().c_str()) = GetParameterText(it).c_str();
			}
		}
}
