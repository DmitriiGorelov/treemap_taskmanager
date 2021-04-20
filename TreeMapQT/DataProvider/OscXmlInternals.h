#pragma once

#include <XML/pugixml.h>
#include <map>

#include "Utils/logMem.h"
#include "XMLParameters/XMLParametrised.h"
#include "XMLParametrisedOsc.h"

namespace global_namespace {

namespace GUI {
#if WITH_FLTK
	namespace
	{
		LOGMEMTYPE logOut(GETLOGMEMINST("oscxml"));
	}

	namespace oscxml {

		/// \brief Converts a type to enother type.
		/// \tparam S Source type which has to be converted.
		/// \tparam D Type the source type has to be converted to.
		/// \param str Given string to be converted.
		/// \return String value converted to type T.
		template <typename D>
		class to
		{
			to(const to&);
			to& operator = (const to&);
		public:
			template <typename S> to(const S& src)
				: m_value(convert(src))
			{
			}

			~to() {}
			operator D() const
			{
				return m_value;
			}
		private:
			template <typename S> D convert(const S& str);

			D m_value;
		};

		const std::string OscXml_Root = "data";		
		
		//std::string ParameterNode("Parameter");
        std::string OscNode("osc");

		namespace Node {
			enum T {
				Osc,
				Parameter,
			};
		};

		typedef std::pair<const std::string, Node::T> TNode_Item;
		typedef std::map<const std::string, Node::T> TNodeMap;


		TNodeMap Node_Map;

		// todo: create instance before read XML ad delete it after: not to occupy memory. (for the moment just checking if can compile the factory or not)
		class TNodeInit
		{
		public:
			TNodeInit()
			{
				Node_Map.insert(TNode_Item(OscNode, Node::Osc));
				Node_Map.insert(TNode_Item(cXMLParametrised::ParameterNode(), Node::Parameter));
			}
		} NodeInit;


		template <> template <> Node::T to<Node::T>::convert<pugi::xml_node>(const pugi::xml_node& xmlNode)
		{
			TNodeMap::iterator item = Node_Map.find(xmlNode.name());
			if (item == Node_Map.end())
			{
				lerr << "illegal node name <" << xmlNode.name() << ">";
				throw std::string("ERROR: illegal node name <") + xmlNode.name() + ">";
			}
			return item->second;
		}

		// Osc ==============================================================================
        std::string OscAttrUID("uid");

		namespace OscAttr
		{
			enum T
            {
                UID,
			};
		};

		typedef std::pair<const std::string, OscAttr::T> TOscAttr_Item;
		typedef std::map<const std::string, OscAttr::T> TOscAttrMap;
		TOscAttrMap OscAttr_Map;

		class TOscAttr
		{
		public:
			TOscAttr()
			{
                OscAttr_Map.insert(TOscAttr_Item(OscAttrUID, OscAttr::UID));
			}
		} OscAttrInit;

		template <> template <> OscAttr::T to<OscAttr::T>::convert<pugi::xml_attribute>(const pugi::xml_attribute& attr)
		{
			TOscAttrMap::iterator item = OscAttr_Map.find(attr.name());
			if (item == OscAttr_Map.end())
			{
				lerr << "illegal Osc node attribute <" << attr.name() << ">";
				throw std::string("ERROR: illegal Osc node attribute <") + attr.name() + ">";
			}
			return item->second;
		}


		// Parameter ==============================================================================
		//std::string ParameterAttrName("Name");
		//std::string ParameterAttrValue("Value");

		namespace ParameterAttr
		{
			enum T
			{
				Name,
				Value
			};
		};

		typedef std::pair<const std::string, ParameterAttr::T> TDeviceParameterAttr_Item;
		typedef std::map<const std::string, ParameterAttr::T> TDeviceParameterAttrMap;
		TDeviceParameterAttrMap DeviceParameterAttr_Map;

		class TDeviceParameterAttr
		{
		public:
			TDeviceParameterAttr()
			{
				DeviceParameterAttr_Map.insert(TDeviceParameterAttr_Item(cXMLParametrised::ParameterAttrName(), ParameterAttr::Name));
				DeviceParameterAttr_Map.insert(TDeviceParameterAttr_Item(cXMLParametrised::ParameterAttrValue(), ParameterAttr::Value));
			}
		} DeviceParameterAttrInit;

		template <> template <> ParameterAttr::T to<ParameterAttr::T>::convert<pugi::xml_attribute>(const pugi::xml_attribute& attr)
		{
			TDeviceParameterAttrMap::iterator item = DeviceParameterAttr_Map.find(attr.name());
			if (item == DeviceParameterAttr_Map.end())
			{
				lerr << "illegal DeviceParameter node attribute <" << attr.name() << ">";
				throw std::string("ERROR: illegal DeviceParameter node attribute <") + attr.name() + ">";
			}
			return item->second;
		}


	}	// namespace opcountxml
#endif
} // GUI
}	// namespace global_namespace
