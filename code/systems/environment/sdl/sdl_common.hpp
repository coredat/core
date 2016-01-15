#ifndef SDL_COMMON_INCLUDED_9E16FD18_21B2_4B3C_9BE0_C15F5A0ADEC4
#define SDL_COMMON_INCLUDED_9E16FD18_21B2_4B3C_9BE0_C15F5A0ADEC4


#include <SDL2/SDL.h>
#include <ostream>


namespace SDL_Detail {


inline void
check_log_error(std::ostream *log)
{
  // Check to see if an error happened.
  const auto err = SDL_GetError();
  if(err && log)
  {
    (*log) << "ERROR: " << (err) << "/n";
  }
}



inline void
log_info(const char *msg, std::ostream *log)
{
  if(log)
  {
    (*log) << msg << "\n";
  }
}


} // ns


#endif // inc guard