
#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GLM\glm.hpp>
//#include <GLM\vec2.hpp>
//#include <GLM\vec4.hpp>

#include <cmath>

#ifndef __MATH_HPP
#define __MATH_HPP

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
}

#endif // !__MATH_HPP
