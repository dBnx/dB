
#include "__Ctrl.hpp"



#ifndef ___DV_SFML_WINDOW_HPP_		// Guard
#define ___DV_SFML_WINDOW_HPP_	

  #ifdef STATIC				// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-window-s-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-window-s.lib")
    #endif // _DEBUG

  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-window-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-window.lib")
    #endif // _DEBUG
  #endif  // STATIC

// Other Libs
#pragma comment (lib, "opengl32.lib")

#endif // !___DV_SFML_WINDOW_HPP_



#include <SFML\Window.hpp>
