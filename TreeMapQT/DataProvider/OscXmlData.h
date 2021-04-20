#pragma once

#include "OscXmlInfo.h"

namespace global_namespace {

	namespace GUI {
#if WITH_FLTK
		class cOscXmlData : public cOscXmlInfo
		{
		public:
			cOscXmlData();
			~cOscXmlData();

		protected:
			//		void Get_SecondsOverall(time_t& Value);
			//		void Set_SecondsOverall(const time_t Value);

			//		void Get_SecondsActual(time_t& Value);
			//		void Set_SecondsActual(const time_t Value);
		};
#endif
	}

}	// end of namespace global_namespace
