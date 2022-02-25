#pragma once

#include "UntypedValue.h"

namespace global_namespace
{	
	template <typename T>
	T tUntypedValue::getTyped() const
	{
		//linfo << typeid(T).name();
		if (modified || _data.type() != typeid(T))
		{
			T t;
			try
			{
				t = boost::lexical_cast<T>(_sValue);
			}
			catch (...)
			{
                std::cout << "Lexical cast error of _data = " << _sValue << ", to type: " << typeid(T).name() << std::endl;
				return T();
			}

			modified = false;
			_data = t;
		}						

		try
		{
			return boost::any_cast<T>(_data);
		}
		catch(...)
		{
			return T();
		}

	}
}
