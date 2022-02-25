#include "XMLParametrisedOsc.h"

#include "XMLParameters/XMLParametrised_Impl.h"

namespace global_namespace {

	namespace GUI {
#if WITH_FLTK

    void cXMLParametrisedOsc::addParameter(const std::string name, const std::string& value) { cXMLParametrised::AddParameter(name, value); }

    double cXMLParametrisedOsc::paraValue() {return cXMLParametrised::GetParameter<double>(AttrNameValue(),100.0); }
    void cXMLParametrisedOsc::paraValue(double v) { cXMLParametrised::SetParameter<double>(AttrNameValue(),v); }

    std::string cXMLParametrisedOsc::paraCaption() {return cXMLParametrised::GetParameterText(AttrNameCaption()); }
    void cXMLParametrisedOsc::paraCaption(const std::string& str) {return cXMLParametrised::SetParameter(AttrNameCaption(),str); }

    std::string cXMLParametrisedOsc::paraText() {return cXMLParametrised::GetParameterText(AttrNameText()); }
    void cXMLParametrisedOsc::paraText(const std::string& str) {return cXMLParametrised::SetParameter(AttrNameText(),str); }

    std::string cXMLParametrisedOsc::paraUser() {return cXMLParametrised::GetParameterText(AttrNameUser()); }
    void cXMLParametrisedOsc::paraUser(const std::string& str) {return cXMLParametrised::SetParameter(AttrNameUser(),str); }

    bool cXMLParametrisedOsc::Selected() { return cXMLParametrised::GetParameter<bool>(AttrNameSelected(), false); }
    void cXMLParametrisedOsc::Selected(bool value) { cXMLParametrised::SetParameter<bool>(AttrNameSelected(), value); }

    bool cXMLParametrisedOsc::Callbacks() { return cXMLParametrised::GetParameter<bool>(AttrNameCallbacks(), false); }
    void cXMLParametrisedOsc::Callbacks(bool value) { cXMLParametrised::SetParameter<bool>(AttrNameCallbacks(), value); }

    int cXMLParametrisedOsc::paraColor() { return cXMLParametrised::GetParameter<int>(AttrNameColor(), 3); }
    void cXMLParametrisedOsc::paraColor(int value) { cXMLParametrised::SetParameter<int>(AttrNameColor(), value); }

    void cXMLParametrisedOsc::setXML(pugi::xml_node& node) { cXMLParametrised::SetXML(node); }

//protected:
    std::string cXMLParametrisedOsc::Selected_str() { return (cXMLParametrised::GetParameter<bool>(AttrNameSelected(), false)) ? "1" : "0"; }
    std::string cXMLParametrisedOsc::Callbacks_str() { return (cXMLParametrised::GetParameter<bool>(AttrNameCallbacks(), false)) ? "1" : "0"; }
    std::string cXMLParametrisedOsc::Color_str() { return to_str::get<int>(cXMLParametrised::GetParameter<int>(AttrNameColor(), (rand() % 255 + 1))); }


#endif
	}
}
