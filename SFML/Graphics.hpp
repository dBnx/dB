
#include "System.hpp"
#include "Window.hpp"



#ifndef ___DV_SFML_GRAPHICS_HPP_		// Guard
#define ___DV_SFML_GRAPHICS_HPP_	

  #ifdef STATIC				// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-graphics-s-d.lib")
    #else
      #pragma comment (lib, "sfml-graphics-s.lib")
    #endif // _DEBUG

  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "sfml-graphics-d.lib")
    #else
      #pragma comment (lib, "sfml-graphics.lib")
    #endif // _DEBUG
  #endif  // STATIC

#ifdef _DEBUG
  #pragma comment (lib, "glew32d.lib")
#else
  #pragma comment (lib, "glew32.lib")
#endif // _DEBUG

#ifdef _DEBUG
  #pragma comment (lib, "freetyped.lib")
#else
  #pragma comment (lib, "freetype.lib")
#endif // _DEBUG

#pragma comment (lib, "jpeg.lib")

#endif // !___DV_SFML_GRAPHICS_HPP_



#include <SFML\Graphics.hpp>
