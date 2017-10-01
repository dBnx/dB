
#include "__Ctrl.hpp"



#ifndef ___DB_SFML_SYSTEM_HPP_		// Guard
#define ___DB_SFML_SYSTEM_HPP_	

  #ifdef STATIC				// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-system-s-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-system-s.lib")
    #endif // _DEBUG

  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-system-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-system.lib")
    #endif // _DEBUG
  #endif  // STATIC

#endif ___DB_SFML_SYSTEM_HPP_		// Guard



#include <SFML\System.hpp>
