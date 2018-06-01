
#include "System.hpp"



#ifndef ___DV_SFML_AUDIO_HPP_		// Guard
#define ___DV_SFML_AUDIO_HPP_	

  #ifdef STATIC	// Link Static
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-audio-s-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-audio-s.lib")
    #endif // _DEBUG

  #else						// Link Dynamic
    #ifdef _DEBUG
      #pragma comment (lib, "SFML\\sfml-audio-d.lib")
    #else
      #pragma comment (lib, "SFML\\sfml-audio.lib")
    #endif // _DEBUG
  #endif  // STATIC

// Other Libs
#pragma comment (lib, "SFML\\openal32.lib")
#pragma comment (lib, "SFML\\ogg.lib")
#pragma comment (lib, "SFML\\flac.lib")
#pragma comment (lib, "SFML\\vorbis.lib")
#pragma comment (lib, "SFML\\vorbisenc.lib")
#pragma comment (lib, "SFML\\vorbisfile.lib")

#endif // !___DV_SFML_AUDIO_HPP_



#include <SFML\Audio.hpp>
