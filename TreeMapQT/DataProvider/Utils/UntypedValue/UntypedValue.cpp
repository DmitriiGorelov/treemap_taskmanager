#include "UntypedValue_Impl.h"

namespace global_namespace
{
	/*std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> > tUntypedValue::get()
	{
		return s;
	}*/	

	template<>
	std::string tUntypedValue::get<std::string>() const
	{
		return _sValue;
	}

	template<>
	int tUntypedValue::get<int>() const
	{
		return getTyped<int>();
	}


	template<>
	float tUntypedValue::get<float>() const
	{
		return  getTyped<float>();
	}

	template<>
	double tUntypedValue::get<double>() const
	{
		return  getTyped<double>();
	}

	template<>
	unsigned long long tUntypedValue::get<unsigned long long>() const
	{
		return  getTyped<unsigned long long>();
	}

	template<>
	unsigned int tUntypedValue::get<unsigned int>() const
	{
		return  getTyped<unsigned int>();
	}

	template<>
	bool tUntypedValue::get<bool>() const
	{
		return  getTyped<bool>();
	}
}