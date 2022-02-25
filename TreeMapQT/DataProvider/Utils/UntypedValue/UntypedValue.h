#pragma once

#include <iostream>
#include <string> 
#include <unordered_map>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>


// more about move constructors and move assignment operators
// https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-160#fromHistory

namespace global_namespace
{	
	class tUntypedValue 
	{
	public:
		tUntypedValue()
			: tUntypedValue("")
		{}

		tUntypedValue(const std::string& v)
			: _sValue(v)
			, modified(true)
			, _data()
		{}

		// Copy constructor.
		tUntypedValue(const tUntypedValue& other)
			: _sValue(other._sValue)
			, modified(true)
			, _data(other._data)
		{
		}

		// Copy assignment operator.
		tUntypedValue& operator=(const tUntypedValue& other)
		{
			if (this != &other)
			{
				// Free the existing resource.			
				_data = other._data;
				_sValue = other._sValue;
				modified = true;
			}
			return *this;
		}

		// Move constructor.
		tUntypedValue(tUntypedValue&& other) noexcept
			: _data(std::move(other)._data)
			, _sValue(std::move(other)._sValue)
			, modified(true)
		{

		}

		// Move assignment operator.
		tUntypedValue& operator=(tUntypedValue&& other) noexcept
		{
			/*std::cout << "In operator=(tUntypedValue&&). length = "
				<< other._length << "." << std::endl;*/

			if (this != &other)
			{
				// Free the existing resource.

				// Copy the data pointer and its length from the
				// source object.
				_data = std::move(other)._data;				
				_sValue = std::move(other)._sValue;
				modified = true;

				other._data.clear();
			}
			return *this;
		}

		~tUntypedValue()
		{
		}

		void set(const std::string& v)
		{
			modified = true;
			_sValue = v;
		}
		
		//std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> > get();
		//std::string get();

		template <typename T>
		T get() const;		

	private:
		template <typename T>
		T getTyped() const; 

	private:
		mutable std::string _sValue;
		mutable bool modified;

		mutable boost::any _data;
	};
}