
#include "__Ctrl.hpp"

#ifndef ___DB_SFGUI_HPP_		// Guard
#define ___DB_SFGUI_HPP_	

  #ifdef STATIC				// Link Static
    #ifndef   SFGUI_STATIC
      #define SFGUI_STATIC
    #endif // SFGUI_STATIC

    #ifdef _DEBUG 
      #pragma comment (lib, "sfgui-s-d.lib")
    #else
      #pragma comment (lib, "sfgui-s.lib")
    #endif // _DEBUG
  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "sfgui-d.lib")
    #else
      #pragma comment (lib, "sfgui.lib")
    #endif // _DEBUG
  #endif  // STATIC

#endif // !___DB_SFGUI_HPP_

#include <SFGUI\SFGUI.hpp>
#include <SFGUI\Widgets.hpp>

#include <dB\SFML\OpenGL.hpp>
