#pragma once

#include "XMLParameters/XMLParametrised.h"
#include "li/include.h"
//#include <XML/pugixml.h>
#include <boost/smart_ptr.hpp>

namespace global_namespace {

	namespace GUI {
#if WITH_FLTK
        class cXMLParametrisedOsc : public cXMLParametrised
		{
            //static std::string AttrNameUID;
            static std::string AttrNameValue() {return "value";}
            static std::string AttrNameCaption() {return "caption";}
            static std::string AttrNameText() {return "text";}
            static std::string AttrNameUser() {return "user";}
            static std::string AttrNameSelected() {return "selected";}
            static std::string AttrNameCallbacks() {return "callbacks";}
            static std::string AttrNameColor() {return "color";}
            //static std::string ParameterNode;
            /*static std::string OscNode;*/

		public:
			static boost::shared_ptr<cXMLParametrisedOsc> Create(const std::string& _name)
			{
				return boost::make_shared<cXMLParametrisedOsc>(_name);
			}

			cXMLParametrisedOsc(const std::string& _name)
				:cXMLParametrised(_name, false)
			{
			}

			~cXMLParametrisedOsc()
			{

			}

			std::string name() { return cXMLParametrised::xmlName(); }

            void addParameter(const std::string name, const std::string& value);

            double paraValue();
            void paraValue(double v);

            std::string paraCaption();
            void paraCaption(const std::string& str);

            std::string paraText();
            void paraText(const std::string& str);

            std::string paraUser();
            void paraUser(const std::string& str);

            bool Selected();
            void Selected(bool value);

            bool Callbacks();
            void Callbacks(bool value);

            int paraColor();
            void paraColor(int value);

            void setXML(pugi::xml_node& node);

		//protected:            
            std::string Selected_str();
            std::string Callbacks_str();
            std::string Color_str();

		private:


		};

		typedef boost::shared_ptr<cXMLParametrisedOsc> pXMLParametrisedOsc;
#endif
	}
}
