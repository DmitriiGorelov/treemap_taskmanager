#pragma once

#include "XMLParameters/XMLParametrised.h"
#include "XMLParameters/XMLParametrised_Internals.h"
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

			void addParameter(const std::string name, const std::string& value) { cXMLParametrised::AddParameter(name, value); }

            double paraValue() {return cXMLParametrised::GetParameter<double>(AttrNameValue(),100.0); }
            void paraValue(double v) { cXMLParametrised::SetParameter<double>(AttrNameValue(),v); }

            std::string paraCaption() {return cXMLParametrised::GetParameterText(AttrNameCaption()); }
            void paraCaption(const std::string& str) {return cXMLParametrised::SetParameter(AttrNameCaption(),str); }

            std::string paraText() {return cXMLParametrised::GetParameterText(AttrNameText()); }
            void paraText(const std::string& str) {return cXMLParametrised::SetParameter(AttrNameText(),str); }

            std::string paraUser() {return cXMLParametrised::GetParameterText(AttrNameUser()); }
            void paraUser(const std::string& str) {return cXMLParametrised::SetParameter(AttrNameUser(),str); }

            bool Selected() { return cXMLParametrised::GetParameter<bool>(AttrNameSelected(), false); }
            void Selected(bool value) { cXMLParametrised::SetParameter<bool>(AttrNameSelected(), value); }

            bool Callbacks() { return cXMLParametrised::GetParameter<bool>(AttrNameCallbacks(), false); }
            void Callbacks(bool value) { cXMLParametrised::SetParameter<bool>(AttrNameCallbacks(), value); }

            int paraColor() { return cXMLParametrised::GetParameter<int>(AttrNameColor(), 3); }
            void paraColor(int value) { cXMLParametrised::SetParameter<int>(AttrNameColor(), value); }

			void setXML(pugi::xml_node& node) { cXMLParametrised::SetXML(node); }

		//protected:            
            std::string Selected_str() { return (cXMLParametrised::GetParameter<bool>(AttrNameSelected(), false)) ? "1" : "0"; }
            std::string Callbacks_str() { return (cXMLParametrised::GetParameter<bool>(AttrNameCallbacks(), false)) ? "1" : "0"; }
            std::string Color_str() { return to_str::get<int>(cXMLParametrised::GetParameter<int>(AttrNameColor(), (rand() % 255 + 1))); }

		private:


		};

		typedef boost::shared_ptr<cXMLParametrisedOsc> pXMLParametrisedOsc;
#endif
	}
}
