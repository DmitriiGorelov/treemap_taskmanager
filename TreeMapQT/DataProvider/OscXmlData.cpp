#include "OscXmlData.h"

#include <string.h>

#include "li/include.h"

#ifndef WIN32
	#include <dirent.h>
	#include <unistd.h>
#endif

namespace global_namespace {
	namespace GUI {
#if WITH_FLTK
		cOscXmlData::cOscXmlData()
            : cOscXmlInfo(appFolder() + std::string("/OscInfo.xml"))
		{
			//linfo << "cOpCountData::ctor()";
		}

		cOscXmlData::~cOscXmlData() {}
#endif
	}
}	// end of namespace global_namespace
