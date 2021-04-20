#pragma once

#include <unordered_map>
#include <list>
#include <XML/pugixml.h>
#include "XMLParameters/XMLParametrised.h"

namespace global_namespace {

	namespace GUI {
#if WITH_FLTK
		class cOscXmlInfo
		{
		public:
			cOscXmlInfo(const std::string& filename);
            ~cOscXmlInfo() { }

		public:
			bool Get_XmlFileName(std::string& FileName);
			bool readXml(std::string xmlFileName);
			bool readXml();
			void clearXml();
			bool modifyXml(std::string xmlFileName);
			bool modifyXml();
	
		private:
			bool read(const pugi::xml_node& node, std::string& width);
            bool modify(pugi::xml_node& node, pXMLParametrised& it_root);
            bool writeOsc(const pugi::xml_node node, std::string& width);
            bool writeParameter(const pugi::xml_node& node, std::string& width);

            void readOsc(const pugi::xml_node& node, std::string& width, pXMLParametrised& osc);
            bool readParameter(const pugi::xml_node& node, std::string& width, pXMLParametrised& osc);
		protected:
            virtual pXMLParametrised AddOsc(const std::string& uid, pXMLParametrised& osc) = 0;
            virtual std::list<pXMLParametrised> getRootOsc() = 0;
		private:
			std::string m_xmlFileName;
			pugi::xml_document 		m_xmlTree;
			pugi::xml_parse_result	m_xmlTreeStatus;

			bool m_xmlReadResult;					///< result of file reading
			bool m_xmlWriteResult;					///< result of file writing
		};
#endif
	}
} // end of namespace global_namespace
