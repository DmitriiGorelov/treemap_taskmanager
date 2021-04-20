#pragma once

#include <list>

#include <boost/shared_ptr.hpp>
#if WITH_BOOST_THREAD
#include <boost/thread/mutex.hpp>
#endif

//#include "shared/Utils/Shared_FX_BaseTypes.h"
//#include "shared/TCP/Shared_FX_DeviceID.h"

//#include "XMLParametrised.h"

//using namespace shared_FX;

namespace global_namespace{
	
	class cXMLParametrised;

namespace XML {	

	typedef std::list<cXMLParametrised*>  pXMLObjectsMap;

	class cXMLContainer
    {
    public:
        cXMLContainer();
        ~cXMLContainer();

        void Register(cXMLParametrised* object);
		void Unregister(cXMLParametrised* object);
		pXMLObjectsMap getObjects();
        //cXMLParametrised getObject(int id);
    private:
        pXMLObjectsMap m_XMLObjects;
		//std::map<cXMLParametrised*, cXMLParametrised*>
    };

	//==============================================================================
	/// \brief cXMLContainer singelton factory

	boost::shared_ptr<cXMLContainer> singleton();

	class XMLContainer
	{
		XMLContainer(const XMLContainer&) ;
		XMLContainer& operator = (const XMLContainer&) ;
	public:
		static boost::shared_ptr<cXMLContainer> instance();

		static boost::shared_ptr<cXMLContainer> singleton();

		~XMLContainer() {}

		static boost::shared_ptr<cXMLContainer> s_singleton;
	};
} //namespace XML
//} // namespace gmas

	typedef boost::shared_ptr<XML::cXMLContainer> p_XMLContainer;

}
