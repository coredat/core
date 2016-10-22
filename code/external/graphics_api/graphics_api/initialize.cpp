#include "initialize.hpp"
#include "ogl/ogl_common.hpp"


namespace Graphics_api {


void
initialize()
{
  Ogl::vao_init();
  Ogl::default_state();
}


void
reset()
{
  Ogl::vao_init();
  Ogl::default_state();
}


} // ns