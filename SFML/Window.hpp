
#include "__Ctrl.hpp"
#include "System.hpp"


#ifndef ___DB_SFML_WINDOW_HPP_		// Guard
#define ___DB_SFML_WINDOW_HPP_	

  #ifdef STATIC				// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-window-s-d.lib")
    #else
      #pragma comment (lib, "sfml-window-s.lib")
    #endif // _DEBUG

  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-window-d.lib")
    #else
      #pragma comment (lib, "sfml-window.lib")
    #endif // _DEBUG
  #endif  // STATIC

// Other Libs
#pragma comment (lib, "opengl32.lib")

#endif // !___DB_SFML_WINDOW_HPP_

#pragma comment (lib, "gdi32.lib")

#include <SFML\Window.hpp>
