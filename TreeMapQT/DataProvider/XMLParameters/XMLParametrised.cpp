
#include "XMLParametrised_Impl.h"
#include "XMLParameterNodes.h"

#include "XMLContainer.h"
#include "../Utils/Casting.h"

#include <algorithm>    // std::sort

#include <boost/smart_ptr.hpp>



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
			auto it = m_Value.find(Cast::lower(name));
			if (name.empty() || it == m_Value.end())
			{				
				return "";
			}						
			
			return (*it).second->getDefault<std::string>();
		}		

        boost::shared_ptr<cXMLParameter> cXMLParametrised::AddParameter(const std::string& name, const std::string& val)
		{
			std::string lowername = Cast::lower(name);

			if (lowername.empty())
			{
                std::cout << "Parameter <> not created!" << std::endl;

				return boost::make_shared<cXMLParameter>();
			}			
			
			auto it = m_Value.find(lowername);
			if (it == m_Value.end())
			{
                m_Value[lowername] = boost::make_shared<cXMLParameter>(val);
                it = m_Value.find(lowername);
			}
			else
				(*it).second->setDefault(val);			

			ParameterChangedCallback(lowername, val);

			return (*it).second;
		}

        boost::shared_ptr<cXMLParameter> cXMLParametrised::GetParameterPtr(const std::string& name)
        {
            std::string lowername = Cast::lower(name);

            if (lowername.empty())
            {
                std::cout << "Parameter <> not created!" << std::endl;

                return boost::make_shared<cXMLParameter>();
            }

            auto it = m_Value.find(lowername);

            return (it != m_Value.end() ? m_Value[lowername] : nullptr);
        }

        void cXMLParametrised::SetParameterPtr(const std::string& name, boost::shared_ptr<cXMLParameter> ptr)
        {
            std::string lowername = Cast::lower(name);

            if (lowername.empty())
            {
                std::cout << "Parameter <> not created!" << std::endl;

                return;
            }

            m_Value[lowername] = ptr;
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
            for (auto& it: m_Value)
			{
				res.push_back(it.first);
			}

            std::sort(res.begin(),res.end());

			return res;
		}

        size_t cXMLParametrised::GetParameterMapSize(const std::string& name, bool makeOutput)
        {
            std::string lowername(Cast::lower(name));
            if (lowername.empty() || m_Value.find(lowername) == m_Value.end())
            {
                if (makeOutput)
                {
                    std::cout << "Parameter " << lowername << " not found" << std::endl;
                }
                return 0;
            }

            return m_Value[lowername]->size();
        }

        template<>
        std::tuple<std::string, std::string> cXMLParametrised::GetParameterMapValueByIndex(const std::string& name, int idx, bool makeOutput)
        {
            std::string lowername(Cast::lower(name));
            if (lowername.empty() || m_Value.find(lowername) == m_Value.end())
            {
                if (makeOutput)
                {
                    std::cout << "Parameter " << lowername << " not found" << std::endl;
                }
                return std::make_tuple("","");
            }

            return m_Value[lowername]->stuple<std::string>(idx);
        }

        std::string cXMLParametrised::xmlName() const
		{
			return m_Name;
		}

		void cXMLParametrised::SetXML(pugi::xml_node& node)
		{            
			for (auto& it : cXMLParametrised::ParameterListNames())
			{                
                auto parameter = GetParameterPtr(it);
                parameter->SetXML(node, it);
			}
		}
}
