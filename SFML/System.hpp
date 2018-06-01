
#include "__Ctrl.hpp"



#ifndef ___DB_SFML_SYSTEM_HPP_		// Guard
#define ___DB_SFML_SYSTEM_HPP_	

  #ifdef STATIC				// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-system-s-d.lib")
    #else
      #pragma comment (lib, "sfml-system-s.lib")
    #endif // _DEBUG

  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-system-d.lib")
    #else
      #pragma comment (lib, "sfml-system.lib")
    #endif // _DEBUG
  #endif  // STATIC

#pragma comment (lib, "winmm.lib")

#endif ___DB_SFML_SYSTEM_HPP_		// Guard



#include <SFML\System.hpp>
