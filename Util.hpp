
#pragma once
#pragma fp_contract (off) // FMA

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif // !_CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <random>
#include <algorithm>
#include "ERET.hpp"

#if !defined(GLM_FORCE_RADIANS) && !defined(GLM_FORCE_DEPTH_ZERO_TO_ONE) 
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif // OWN_USE_GLEW
#include <GLM\vec2.hpp>
#include <GLM\vec3.hpp>

#ifdef INC_TESTBENCH
#include <GLM\vec2.hpp>

#include <iomanip>
#include <iostream>
#endif // INC_TESTBENCH

#ifndef __DB_UTIL_HPP
#define __DB_UTIL_HPP

namespace dB
{
	// West-Constable Raw C-Pointer
	// const ptr< const type > ptr;
	// instead of
	// type const * const      ptr;
	// Allows a consistent design and avoids the awful const behaviour.  
	template< typename T >
	using ptr = T * ;

	template< typename T = double>
	class HighResTimer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _last;

		constexpr
		T	_Update_Or_Difference(const unsigned update, const unsigned calc_dur = 0) 
		{
			using clk_highres = std::chrono::high_resolution_clock;
			//typedef std::chrono::duration<T, Ratio> DurCast;
			typedef std::chrono::duration<T> DurCast;


			auto now = std::chrono::high_resolution_clock::now();
			T duration{};

			if (calc_dur)
			{
				duration = std::chrono::duration_cast<DurCast>(now - this->_last).count();
			}

			if (update)
			{
				this->_last = now;
			}
			return duration;
		}

	public:

		HighResTimer() : _last(std::chrono::high_resolution_clock::now())
		{

		}
		~HighResTimer()
		{

		}

		void Update()
		{
			this->_Update_Or_Difference(1, 0);
		}

		constexpr
		T	Difference() 
		{
			return this->_Update_Or_Difference(0, 1);
		}

		constexpr
		T	Update_Difference()
		{
			return this->_Update_Or_Difference(1, 1);
		}

		HighResTimer& operator = (const HighResTimer& other)
		{
			if (this != &other)
			{
				this->_last = other->_last;
			}
		}
	};

	class FPSTool
	{
	private:
		dB::HighResTimer<double>	t;
		size_t						frames{ 0 };
		size_t						fps_limit{ 0 };

		constexpr float Frames()
		{
			return static_cast<float>(this->frames);
		}

	public:
		FPSTool(size_t limit = 0) : fps_limit(limit)
		{

		}

		~FPSTool()
		{

		}

		void Reset()
		{
			this->frames = 0;
			this->t.Update();
		}

		constexpr bool HasTimePassed(double t = 1.f)
		{
			return (this->t.Difference() > t) ? true : false;
		}

		double GetFPS()
		{
			return this->Frames() / this->t.Difference();
		}

		double GetSPF()
		{
			return this->t.Difference() / this->Frames();
		}

		void FrameInc()
		{
			this->frames++;
		}

	};
}

namespace dB::untested
{
	// path = 0.f ... 1.f
	glm::vec3 V3Interpolate(glm::vec3 p0, glm::vec3 p1, float t = .5f)
	{
		return (p1 - p0) * t + p0;
	}

	glm::vec3 V3Interpolate(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float t)
	{
		const float b0 = std::powf(1.f - t, 2.f);
		const float b1 = 2.f * t * (1.f - t);
		const float b2 = std::powf(t, 2.f);
		return p0 * b0 + p1 * b1 + p2 * b2;
	}


}

namespace dB::Math
{

	// http://forum.codecall.net/topic/41540-fibonacci-with-no-recursion-for-fun/

	int factorial(const int n) {
		int fact = 1;
		for (int i = 1; i <= n; i++) fact *= i;
		return fact;
	}

	int permutation(const int n, const int r) {
		int perm = 1;
		for (int i = n - r + 1; i <= n; i++) perm *= i;
		return perm;
	}

	int combination(const int n, const int r) {
		return permutation(n, r) / factorial(r);
	}

	//Non-recrusive: O(1)
	uint64_t Fib_NonRecursive(const uint64_t n)
	{
		//return (1 / sqrt(5)) * (pow(((1 + sqrt(5)) / 2), n) - pow(((1 - sqrt(5)) / 2), n));
		const double dn = static_cast <double> (n);
		return static_cast <uint64_t> ( (1. / sqrt(5.)) * (pow(((1. + sqrt(5.)) / 2.), dn) - pow(((1. - sqrt(5.)) / 2.), dn)) );
	}

	//Iterative: O(n)
	uint64_t Fib_Iterative(const uint64_t n)
	{
		uint64_t u = 0;
		uint64_t v = 1;
		uint64_t i, t;

		for (i = 2; i <= n; i++)
		{
			t = u + v;
			u = v;
			v = t;
		}
		return v;
	}

	uint64_t Fib(const uint64_t n)
	{
		return Fib_NonRecursive(n);
	}

#ifdef INC_TESTBENCH
	namespace _testbench
	{
		glm::dvec2 Benchmark_Fib_Single(const int argument = 100, const size_t iterations = 16000)
		{
			double dt, dt_avg;
			dB::HighResTimer<double> t;
			uint64_t result;
			glm::dvec2 ret;

			dt_avg = 0.;
			for (size_t i = 0; i < iterations; i++)
			{
				t.Update();
				result = dB::Math::Fib_Iterative(argument);
				dt = t.Difference();
				dt_avg += dt / static_cast<double>(iterations);
			}
			//cout << "Fib_Iterative    : Time    = " << setw(12) << dt_avg * 10E9 << " ns\n";
			ret.x = dt_avg;

			dt_avg = 0.;
			for (size_t i = 0; i < iterations; i++)
			{
				t.Update();
				result = dB::Math::Fib_NonRecursive(argument);
				dt = t.Difference();
				dt_avg += dt / static_cast<double>(iterations);
			}
			ret.y = dt_avg;
			//cout << "Fib_NonRecursive : Time    = " << setw(12) << dt_avg * 10E9 << " ns\n\n";
			return ret;
		}

		void Benchmark_Fib(const size_t max = 94, const size_t it = 16000)
		{
			using namespace std;

			auto format = [](const glm::dvec2 &arg)
			{
				std::stringstream ss;
				ss << setw(9) << arg.x * 10E9 << " ns | " << setw(9) << arg.y * 10E9 << " ns | d = " << setw(9) << std::abs(arg.x - arg.y) * 10E9 << " ns | *" << setw(8) << std::max(arg.x, arg.y) / std::min(arg.x, arg.y) << " \n";
				return ss.str();
			};

			glm::dvec2 ret, ret_avg;

			cout << "  Fib_Iterative   | Fib_NonRecursive\n";
			for (size_t i = 0; i < max; i++)
			{
				//cout << "Fib_Iterative    : Time    = " << setw(12) << dt_avg * 10E9 << " ns\n";
				ret = Benchmark_Fib_Single(i, it);
				cout << setw(3) << i << ") " << format(ret);
				ret_avg += ret / static_cast<double>(max);
			}
			cout << "  Fib_Iterative   | Fib_NonRecursive\n";
			cout << "Avg: " << format(ret_avg) << "\n";
		}
	}
#endif // INC_TESTBENCH
}

#endif // !__DB_UTIL_HPP
