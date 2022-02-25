//#include "XMLParameter.h"
#include "XMLParameter_Impl.h"

#include "../Utils/Casting.h"

namespace global_namespace
{	
	/*template<>
	std::string cXMLParameter::getDefault()
	{
		return m_Default.get<std::string>();
	}*/

	void cXMLParameter::setDefault(const std::string& value)
	{
		m_Default.set(value);
	}

	void cXMLParameter::addValueForKey(const std::string& key, const std::string& value)
	{
		m_ValuesForKey[Cast::lower(key)].push_back(value);
	}

	template<>
	std::tuple<std::string, std::string> cXMLParameter::tuple(int idx)
	{
		auto it = m_ValuesForKey.begin();
		std::advance(it, idx);

		std::string s("");
		if (it->second.size() > 0)
			s = it->second[0].get<std::string>();

		return std::make_tuple(it->first, s);
	}	
}
