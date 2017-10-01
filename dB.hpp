
#include <own\Include.hpp>
#include <own\ERET.hpp>
#include <own\Math.hpp>
#include <own\Util.hpp>

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