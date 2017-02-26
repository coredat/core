#ifndef DIRECTORIES_INCLUDED_96607AB1_193A_4C33_B039_5103BDC26B1D
#define DIRECTORIES_INCLUDED_96607AB1_193A_4C33_B039_5103BDC26B1D


#include <utilities/directory.hpp>
#include <utilities/platform.hpp>
#include <string.h>


namespace Common {
namespace Dir {


inline const char *
assets()
{
  static char asset_path[LIB_MAX_FILE_PATH_SIZE]{0};
  
  if(strlen(asset_path) == 0)
  {
    strcat(asset_path, lib::dir::exe_path());
    
    #ifdef LIB_PLATFORM_MAC
    strcat(asset_path, "../Resources/");
    #endif
    
    strcat(asset_path, "assets/");
  }
  
  return asset_path;
}


} // ns
} // ns


#endif // inc guard
