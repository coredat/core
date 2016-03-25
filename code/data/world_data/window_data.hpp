#ifndef WINDOW_DATA_INCLUDED_095CB3FC_7155_4B2C_993C_C45D94EC21C9
#define WINDOW_DATA_INCLUDED_095CB3FC_7155_4B2C_993C_C45D94EC21C9


#include <stdint.h>


namespace World_data {


struct Application_window
{
  uint32_t        width         = 0;
  uint32_t        height        = 0;
  bool            is_fullscreen = false;
};


void
application_window_create(Application_window *app, );


} // ns


#endif // inc guard