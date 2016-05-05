#ifndef CONTEXT_INCLUDED_46DB9270_5D90_4D03_8D7E_E8D2EE29EA2B
#define CONTEXT_INCLUDED_46DB9270_5D90_4D03_8D7E_E8D2EE29EA2B


#include <graphics_api/pixel_format.hpp>


namespace Core {


struct Context_setup
{
  bool vsync = false;
  Graphics_api::Pixel_format backbuffer_fmt = Graphics_api::Pixel_format::rgba8;
};


} // ns


#endif // inc guard