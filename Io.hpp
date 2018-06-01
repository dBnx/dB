
#pragma once

#include <dB\Include.hpp>

#ifndef __DB_IO_HPP
#define __DB_IO_HPP

// TODO: Add other implementations of ReadFileStr
//       Add benchmark
//       Add ReadFileBin
//       ReadFile Wrapper

namespace dB::io
{
	namespace __implementation
	{
		std::optional<std::string> ReadFileStr_stringstream(std::string_view fileName)
		{
			std::stringstream	buffer;
			std::string			line;

			std::string content{};
			std::ifstream ifstr(fileName.data());

			if (! ifstr.good() || ifstr.eof())
				return {};

			while (std::getline(ifstr, line))
				buffer << line;

			return buffer.str();
		}
	}

	std::optional<std::string> ReadFileStr(const std::string_view fileName)
	{
		return __implementation::ReadFileStr_stringstream(fileName);
	}
}

/*
template < const size_t repetitions = 20, const char* filename >
void Bench()
{
	dB::HighResTimer<> t;
	std::optional<std::string> str;

	double tMax{}, tMin{}, tAvg{}, dt{};

	for (size_t i = 0; i < repetitions; i++)
	{
		std::cout << i << " ";
		t.Update();
		str = dB::io::ReadFileStr(filename);
		dt = t.Difference();

		tMax = std::max(tMax, dt);
		tMin = std::min(tMin, dt);
		tAvg += dt;
	}
	tAvg /= static_cast<double>(repetitions);

	std::cout << "\n"
		<< "t (avg) = " << tAvg * 1e3 << " ms\n"
		<< "t (min) = " << tMin * 1e3 << " ms\n"
		<< "t (max) = " << tMax * 1e3 << " ms\n";
}
*/
#endif // !__DB_IO_HPP