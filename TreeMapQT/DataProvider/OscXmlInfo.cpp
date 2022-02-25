
#include "OscXmlInfo.h"
#include "OscXmlInternals.h"

#include "Utils/logMem.h"

#include <XML/pugixml.h>
#include <fstream>
#include <QDebug>

namespace global_namespace {
	namespace GUI {
#if WITH_FLTK
		cOscXmlInfo::cOscXmlInfo(const std::string& filename)
			: m_xmlFileName(filename)
			, m_xmlTree()
			, m_xmlTreeStatus()
			, m_xmlReadResult(false)
			, m_xmlWriteResult(false)
		{
			//logOut
			/*linfo << "--- read cOscXmlInfo info from file '" << filename << "'" << std::endl;
			if (!readXml(filename))
				lerr << "--- read cOscXmlInfo info finished with error. Using default values." << std::endl;
			else
				linfo << "--- read cOscXmlInfo info finished successful ---" << std::endl;
				*/

			/*linfo << "--- modify cOscXmlInfo info file '" << filename << "' with actual informations" << std::endl;
			if (!modifyXml(filename))
				lerr << "--- modify cOscXmlInfo info finished with error. Informations not updated" << std::endl;
			else
				linfo << "--- modify cOscXmlInfo info finished successful ---" << std::endl;
			*/
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		bool cOscXmlInfo::Get_XmlFileName(std::string& xmlFileName)
		{
			xmlFileName = m_xmlFileName;
			return true;
		}

		bool cOscXmlInfo::readXml()
		{
			return readXml(m_xmlFileName);
		}

		bool cOscXmlInfo::readXml(std::string xmlFileName)
		{
			m_xmlFileName = xmlFileName;	// file name to read
			m_xmlReadResult = true;			// assuming, we will be successful

			std::ifstream configFile(xmlFileName.c_str());
			if (!configFile.is_open())
			{
				m_xmlReadResult = false;
				XMLERROR << "xml file <" << xmlFileName << "> can't be opened" << std::endl;
                qDebug() << xmlFileName.c_str();
			}
			else
			{
				//pugi::xml_document xmlTree;
				//pugi::xml_parse_result result = xmlTree.load(configFile);
				//if (result.status != pugi::status_ok)
				m_xmlTreeStatus = m_xmlTree.load(configFile);
				if (m_xmlTreeStatus.status != pugi::status_ok)
				{
					m_xmlReadResult = false;
					XMLERROR << "xml file <" << xmlFileName << "> may be corrupt" << std::endl;
				}
				else
				{
					std::string width;
					if (!read(m_xmlTree.first_child(), width))
					{
						XMLERROR << "xml file <" << xmlFileName << "> read failed" << std::endl;
					}
					else
					{
                        XMLERROR << "xml file <" << xmlFileName << "> read finished successful" << std::endl;
					}
				}
			}

			// Just a proposal. If XML does not exist, we create its hierarchie manually.
			if (!m_xmlReadResult)
			{
				clearXml();
				/*auto root = m_xmlTree.append_child(oscxml::OscXml_Root.c_str());
				auto child = root.append_child(oscxml::SecondsOverall_Node.c_str());
				child.append_attribute(oscxml::SecondsOverallAttrValue.c_str()) = m_SecondsOverallValue;
				child.append_attribute(oscxml::SecondsOverallAttrType.c_str()) = m_SecondsOverallType.c_str();
				child.append_attribute(oscxml::SecondsOverallAttrDescription.c_str()) = m_SecondsOverallDescription.c_str();

				auto child2 = root.append_child(oscxml::SecondsActual_Node.c_str());
				child2.append_attribute(oscxml::SecondsActualAttrValue.c_str()) = m_SecondsActualValue;
				child2.append_attribute(oscxml::SecondsActualAttrType.c_str()) = m_SecondsActualType.c_str();
				child2.append_attribute(oscxml::SecondsActualAttrDescription.c_str()) = m_SecondsActualDescription.c_str();*/
				m_xmlReadResult = true;
			}
			return m_xmlReadResult;
		}

		bool cOscXmlInfo::read(const pugi::xml_node& node, std::string& width)
		{
			for (auto& node : node.children())
			{		
				switch (oscxml::to<oscxml::Node::T>(node))
				{
					case oscxml::Node::Osc:
                    {
                        pXMLParametrised osc;
                        readOsc(node, width, osc);
						break;				
                    }                    
					default:
						XMLERROR << "unsupported node <" << (node).name() << ">";
						throw std::string("ERROR: unsupported node <") + (node).name() + ">";
				}
			}

			
			return m_xmlReadResult;
		}

        void cOscXmlInfo::readOsc(const pugi::xml_node& node, std::string& width, pXMLParametrised& osc)
		{
            std::string uid("");
			for (const auto& attr : node.attributes())
			{
				switch (oscxml::to <oscxml::OscAttr::T>(attr))
                {
                case oscxml::OscAttr::UID:
                    uid = attr.as_string();
					break;
				default:
					XMLERROR << "SecondsOverall attribute <" << (attr).name() << "> not supported" << std::endl;
					return;
					//break;
				}
            }

            pXMLParametrised p = AddOsc(uid, osc);

			if (p)
			{
				typedef pugi::xml_object_range<pugi::xml_node_iterator> TList;
				TList list = node.children();
				for (auto& node : list)
				{
					switch (oscxml::to <oscxml::Node::T>(node))
					{
					case oscxml::Node::Parameter:
						readParameter(node, width, p);
						break;
                    case oscxml::Node::Osc:
                        readOsc(node, width, p);
                        break;
					default:
						XMLERROR << "node <" << node.name() << "> not supported" << std::endl;
						break;
					}
				}
			}
		}

        bool cOscXmlInfo::readParameter(const pugi::xml_node& node, std::string& width, pXMLParametrised& osc)
		{
			std::string name("");
			std::string value("");

			for (const auto& attr : node.attributes())
			{
				oscxml::ParameterAttr::T actualAttrType = oscxml::to <oscxml::ParameterAttr::T>(attr);
				switch (actualAttrType)
				{
					case oscxml::ParameterAttr::Value:
						value = attr.as_string();
						break;
					case oscxml::ParameterAttr::Name:
						name = attr.as_string();
						break;
					default:
						XMLERROR << "SecondsOverall attribute <" << (attr).name() << "> not supported" << std::endl;
						return false;
						//break;
				}
			}
            osc->AddParameter(name,value);

			return true;
        }

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void cOscXmlInfo::clearXml()
		{
			m_xmlTree.reset();
			m_xmlTree.append_child(oscxml::OscXml_Root.c_str());
		}

		bool cOscXmlInfo::modifyXml()
		{
            return modifyXml(m_xmlFileName);
        }

		bool cOscXmlInfo::modifyXml(std::string xmlFileName)
		{
			m_xmlFileName = xmlFileName;	// file name to modify
			m_xmlReadResult = true;			// assuming, we will be successful
			m_xmlWriteResult = true;		// assuming, we will be successful

			std::ofstream configFile(xmlFileName.c_str());
			if (!configFile.is_open())
			{
				m_xmlReadResult = false;
				m_xmlWriteResult = false;
                XMLERROR << "xml file <" << xmlFileName << "> can't be opened" << std::endl;
                qDebug() << xmlFileName.c_str();
			}
			else
			{
				//pugi::xml_document xmlTree;
				//pugi::xml_parse_result result = xmlTree.load(configFile);
				//if (result.status != pugi::status_ok)
				/*if (m_xmlTreeStatus.status != pugi::status_ok)
				{
					m_xmlReadResult = false;
                    XMLERROR << "xml file <" << xmlFileName << "> hasn't been loaded yet " << std::endl;
				}
				else*/
				{
					clearXml();
                    pugi::xml_node node=m_xmlTree.first_child();                    
                    for (auto& it_root : getRootOsc())
                    {
                        auto osc = node.append_child(oscxml::OscNode.c_str());
                        for (auto& it : getUsers())
                        {
                            auto para = osc.append_child("user");
                            para.append_attribute("n") = it.c_str();
                        }
                        osc.append_attribute(oscxml::OscAttrUID.c_str()) = it_root->xmlName().c_str();

                        it_root->SetXML(osc);

                        modify(osc, it_root);
                    }

                    if (!m_xmlTree.save_file(xmlFileName.c_str(), PUGIXML_TEXT("\t")))
                    {
                        XMLERROR << "xml file <" << xmlFileName << "> write failed" << std::endl;
                    }
                    else
                    {
                        //linfo << "xml file <" << xmlFileName << "> write finished successful" << std::endl;
                    }
				}
			}

			return (m_xmlReadResult && m_xmlWriteResult);
        }

        bool cOscXmlInfo::modify(pugi::xml_node& node, pXMLParametrised& it_root)
        {                        
            for (auto& it : it_root->getChildrenOsc())
            {
                auto osc = node.append_child(oscxml::OscNode.c_str());
                osc.append_attribute(oscxml::OscAttrUID.c_str()) = it->xmlName().c_str();

                it->SetXML(osc);

                modify(osc, it);
            }
			
			return true;
		}	

        /*pXMLParametrisedOsc cOscXmlInfo::AddOsc(const std::string& map_name, const std::string& name)
		{			
            if (m_vOsc.find(map_name) != m_vOsc.end())
			{
				for (auto& it : m_vOsc[map_name])
				{
					if (it->name() == name)
					{
						return it;
					}
				}				
			}

			pXMLParametrisedOsc p = cXMLParametrisedOsc::Create(name);
			m_vOsc[map_name].emplace_back(p);
			return p;
		}

		pXMLParametrisedOsc cOscXmlInfo::GetOsc(const std::string& map_name, const std::string& name)
		{
			return AddOsc(map_name, name);
        }*/
#endif
	}
} // end of namespace global_namespace
