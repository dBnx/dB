
#include "System.hpp"
#include "Window.hpp"



#ifndef ___DV_SFML_GRAPHICS_HPP_		// Guard
#define ___DV_SFML_GRAPHICS_HPP_	

  #ifdef STATIC				// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-graphics-s-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-graphics-s.lib")
    #endif // _DEBUG

  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-graphics-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-graphics.lib")
    #endif // _DEBUG
  #endif  // STATIC

// Other Libs
#pragma comment (lib, "SFML\\freetype.lib")
#pragma comment (lib, "SFML\\jpeg.lib")

#endif // !___DV_SFML_GRAPHICS_HPP_



#include <SFML\Graphics.hpp>
