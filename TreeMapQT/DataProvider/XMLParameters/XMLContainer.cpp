#include "XMLContainer.h"

#include <boost/make_shared.hpp>

#include "XMLParametrised.h"
#include "XMLParametrised_Internals.h"

namespace global_namespace{
//namespace gmas {
	namespace XML {

	//boost::shared_ptr<cXMLContainer> XMLContainer;

	//-----------------------------------------------------------------------------
	cXMLContainer::cXMLContainer()
		: m_XMLObjects()

	{
	}

	//-----------------------------------------------------------------------------
	cXMLContainer::~cXMLContainer()
	{
		std::cout << "cXMLContainer destroyed!" << std::endl;
	}


	//template <class TFXDevice> void cCommTCP::AddIFXDevice(const TFXDevice& device)
	void cXMLContainer::Register(cXMLParametrised* object)
	{			
		m_XMLObjects.emplace_back(object);
	}

	void cXMLContainer::Unregister(cXMLParametrised* object)
	{
		for (auto it = m_XMLObjects.begin(); it!= m_XMLObjects.end(); ++it)
		{
			if ((*it) == object)
			{
				m_XMLObjects.erase(it);
				break;
			}
		}
	}

	std::list<cXMLParametrised*> cXMLContainer::getObjects()
	{
		return m_XMLObjects;
	}    

	// ============================================================ singleton
	p_XMLContainer XMLContainer::s_singleton;

#if WITH_BOOST_THREAD
    boost::mutex m_mutexIFXContainer;
#endif

	p_XMLContainer XMLContainer::instance()
	{
#if WITH_BOOST_THREAD
		boost::unique_lock<boost::mutex> lock(m_mutexIFXContainer);
#endif
		if (!s_singleton)
			s_singleton = boost::make_shared<cXMLContainer>();
		return s_singleton;
	}

	p_XMLContainer XMLContainer::singleton()
	{
#if WITH_BOOST_THREAD
		boost::unique_lock<boost::mutex> lock(m_mutexIFXContainer);
#endif
		if (!s_singleton)
		{
			//lerr << "IXMLContainer::instance never called!" << std::endl;
			throw "XMLContainer::instance never called!";
		}
		return s_singleton;
	}

	p_XMLContainer singleton()
	{
		return XMLContainer::singleton();
	}

} //namespace XML

//} // namespace gmas

} // namespace global_namespace
