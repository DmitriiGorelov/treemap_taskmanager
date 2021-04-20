
#pragma once

#include "XMLParametrised.h"
#include "XMLParametrised_Internals.h"

#include <boost/weak_ptr.hpp>

namespace global_namespace {			

	class XMLParametrizedAddon {
		typedef boost::weak_ptr<cXMLParametrised> pXMLParametrisedWeak;
	public:
		XMLParametrizedAddon(const pXMLParametrised& _value)
			: m_pValue(_value)
		{

		}

		~XMLParametrizedAddon()
		{
			
		}

		void xmlPara(const pXMLParametrised& value) { m_pValue = value; }
		pXMLParametrised xmlPara() { return m_pValue.lock(); }

	private:
		pXMLParametrisedWeak m_pValue;
	};

}