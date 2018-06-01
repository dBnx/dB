
#ifndef NO_STATIC

  #ifndef STATIC
    #define STATIC
  #endif // !STATIC

  #ifdef STATIC
    #ifndef SFML_STATIC
      #define SFML_STATIC
    #endif // !SFML_STATIC
  #endif // STATIC

#endif // !NO_STATIC
