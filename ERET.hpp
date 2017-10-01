
#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include <optional>
#include <iostream>
#include <iomanip>

#include <cstdint>
#include <ctime>

#ifdef __GNUC__

#ifndef likely
#define likely(x)       __builtin_expect(!!(x),1)
#endif // !likely

#ifndef unlikely
#define unlikely(x)     __builtin_expect(!!(x),0) // https://stackoverflow.com/questions/39029158/using-likely-unlikely-preprocessor-macros-in-if-else-if-chain
#endif // !unlikely

#else

#ifndef likely
#define likely(x)       (x)
#endif // !likely

#ifndef unlikely
#define unlikely(x)     (x)
#endif // !unlikely

#endif // !__GNUC__



#ifndef   __ERET_HPP
#define   __ERET_HPP

namespace dB
{
	std::string GetTimeStamp()
	{
		time_t	rawtime;
		char	buffer[80];

		time(&rawtime);

		strftime(buffer, sizeof(buffer), "%Y-%m-%d %T", localtime(&rawtime));
		return std::string(buffer);
	}

	// TODO
	// used to handle multiple internal
	// error return values 
	//typedef std::function<void(Eret& eret)> EretHandler; 

	typedef struct _Eret
	{

		int32_t			code{};
		std::string		msg{};
		//std::optional<std::string>	obj{};

		_Eret(const int32_t errorCode = 0, const  std::string errorMsg = "") : code(errorCode), msg(errorMsg)
		{ }

		_Eret(const std::string errorMsg, const int32_t errorCode = 0) : code(errorCode), msg(errorMsg)
		{ }

		_Eret(const _Eret eret, const int32_t errorCode) : code(errorCode), msg(eret.msg)
		{ }

		std::string Log_Str()
		{
			std::stringstream ss;

			ss << GetTimeStamp();

			if (!this->code)
				ss << "   Information -  ";
			else if (this->code > 0)
				ss << " ~ Warning     -  ";
			else
				ss << " x Error       -  ";

			if (this->code)
				ss << "Errorcode : " << std::setw(10) << this->code;

			ss << "  # " << this->msg << std::flush;

			return ss.str();
		}

		template <typename functor>
		_Eret& Run_If_Error(functor function)
		{
			if (this->code < 0)
				function();
			return *this;
		}

		template <typename functor>
		_Eret& Run_If_Success(functor function)
		{
			if (this->code == 0)
				function();
			return *this;
		}

		_Eret& Log(const bool output_error_also_to_cout = false)
		{
			if (this->code < 0)
			{
				if (output_error_also_to_cout)
					std::cout << this->Log_Str() << "\n";
				std::cerr << this->Log_Str() << "\n";
			}
			else
				std::cout << this->Log_Str() << "\n";

			return *this;
		}
		_Eret& Log_Cout()
		{
			std::cerr << this->Log_Str() << "\n";
			return *this;
		}
		_Eret& Log_Cerr()
		{
			std::cout << this->Log_Str() << "\n";
			return *this;
		}
		_Eret& Log_File(std::ofstream& of)
		{
			if (of.is_open())
				of << this->Log_Str() << "\n";
			return *this;
		}
		_Eret& Abort_If_Error()
		{
			if (this->code < 0)
				abort();
			return *this;
		}
		_Eret& Exit_If_Error()
		{
			if (this->code < 0)
				exit(this->code);
			return *this;
		}
	} Eret;
}

namespace dB
{
	template <typename T>
	class _Eret_Pair
	{
	private:
		T		value;
		Eret	eret;
	public:
		_Eret_Pair(T value = T{}, 
			Eret eret = dB::Eret() ) : value(value), eret(eret)
		{

		}

		~_Eret_Pair()
		{

		}

		// Test
		template <typename Functor>
		_Eret_Pair& F_Eret(Functor func)
		{
			func(this->eret);
			return *this;
		}

		_Eret_Pair& Value(const T value)
		{
			this->value = value;
			return *this;
		}

		T Value() const
		{
			return this->value;
		}

		_Eret_Pair& Eret(const Eret eret)
		{
			this->eret = eret;
			return *this;
		}

		dB::Eret Eret() const
		{
			return this->eret;
		}
	};
	/*
	template <typename T>
	typedef _Eret_Pair<T> Eret_Pair<T>;*/
}

#endif // !__ERET_HPP

/*
template <class T>
struct _Eret_And_Return_Value
{
std::pair<Eret, T> ret;

_Eret_And_Return_Value(Eret eret, T value)
{

}
_Eret_And_Return_Value& EretFunc(Eret& (Eret::*func)())
{
(this->ret.first.*func)();
//f(this->ret.first);
return *this;
}

_Eret_And_Return_Value& Log()
{
this->ret.first.Log();
return *this;
}
_Eret_And_Return_Value&  Abort_If_Error()
{
this->ret.first.Abort_If_Error();
return *this;
}
Eret& Eret()
{
return this->ret.first;
}
T& Value()
{
return this->ret.second;
};
};


template <class T>
using EretPair = _Eret_And_Return_Value;


template <class T>
using _EretPair = _Eret_And_Return_Value;

template <class T>
using EretPair(e, t) = _EretPair<decltype(t)> (Eret e, T t);


cout << EretPair<std::string>(Eret("Test", -1), "Value") \
.Log().Abort_If_Error().Value() << endl;
*/
