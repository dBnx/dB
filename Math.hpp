
#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#if ! __has_include("GLM\glm.hpp")
# error Couldn't include GLM.
#endif

#include <GLM\glm.hpp>

#include <cmath>

#ifndef __DB_MATH_HPP
#define __DB_MATH_HPP

namespace dB
{
	namespace ___intern
	{
		template <class T>
		std::string _format_single_var(const std::string varName, T value, int width = 8)
		{
			std::stringstream ss;
			ss << varName << " = " << std::setw(8) << value << " ";
			return ss.str();
		}
	}

	const std::string Format(const glm::vec2 v)
	{
		std::stringstream ss;
		ss << ___intern::_format_single_var("X", v.x)
			<< ___intern::_format_single_var("Y", v.y);
		return ss.str(); 
	}

	const std::string Format(const glm::vec4 v)
	{
		std::stringstream sst;
		sst << ___intern::_format_single_var("X", v.x)
			<< ___intern::_format_single_var("Y", v.y)
			<< ___intern::_format_single_var("Z", v.z)
			<< ___intern::_format_single_var("W", v.w);
		return sst.str();
	}

	template <typename T>
	constexpr T Sigmoid(T x)
	{
		return T(1) / (T(1) + std::pow(M_E, -x));
	}

	template <typename T>
	constexpr T ReLU(T x)
	{
		return std::max(0, x);
	}

	namespace hash
	{
		namespace ___intern
		{
			// http://www.isthe.com/chongo/tech/comp/fnv/
			// https://de.wikipedia.org/wiki/FNV_(Informatik)
			// https://fnvhash.github.io/fnv-calculator-online/

			const uint64_t prime32 = 16777619;
			const uint64_t basis32 = 2166136261;

			const uint64_t prime64 = 0x00000100000001b3;
			const uint64_t basis64 = 0xcbf29ce484222325;

			// ERROR: TODO: Values are not correct.
			template <typename T>
			T core_FNV(const T prime, const T basis, const uint8_t* data, const size_t size)
			{
				T Hash = basis;
				for (size_t i = 0; i < size; i++)
				{
					Hash = ( Hash ^ *(data+i) ) * prime;
				}
				
				return Hash;
			}
		}

		// 64 bit Versions
		template < typename T>
		uint64_t FNV64(T* data, size_t size)
		{
			namespace ins = ___intern;
			return ins::core_FNV<uint64_t>(ins::prime64, ins::basis64, (uint8_t*)data, size);
		}

		template < typename _InIt>
		uint64_t FNV64(_InIt _First, _InIt _Last)
		{
			uint8_t* data = &*_First;//uint8_t*(in[0]);

			namespace ins = ___intern;
			return ins::core_FNV<uint64_t>(ins::prime64, ins::basis64, data, std::distance(_First, _Last));
		}

		template < typename T, size_t SIZE>
		uint64_t FNV64(std::array<T, SIZE> in)
		{
			uint8_t* data = uint8_t*(in[0]);

			namespace ins = ___intern;
			return ins::core_FNV<uint64_t>(ins::prime64, ins::basis64, data, in.size());
		}

		// 32 bit Versions
		template < typename T>
		uint64_t FNV32(T* data, size_t size)
		{
			namespace ins = ___intern;
			return ins::core_FNV<uint32_t>(ins::prime64, ins::basis64, (uint8_t*)data, size);
		}

		template < typename _InIt>
		uint64_t FNV32(_InIt _First, _InIt _Last)
		{
			uint8_t* data = &*_First;//uint8_t*(in[0]);

			namespace ins = ___intern;
			return ins::core_FNV<uint32_t>(ins::prime64, ins::basis64, data, std::distance(_First, _Last));
		}

		template < typename T, size_t SIZE>
		uint64_t FNV32(std::array<T, SIZE> in)
		{
			uint8_t* data = uint8_t*(in[0]);

			namespace ins = ___intern;
			return ins::core_FNV<uint32_t>(ins::prime64, ins::basis64, data, in.size());
		}
	}
}

#endif // !__DB_MATH_HPP
