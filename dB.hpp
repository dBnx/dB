
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#ifndef __DB_DB_HPP
#define __DB_DB_HPP

#include <dB\Include.hpp>
#include <dB\ERET.hpp>
#include <dB\Math.hpp>
#include <dB\Util.hpp>

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

#endif // !__DB_DB_HPP
