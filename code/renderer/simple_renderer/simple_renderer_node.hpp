#ifndef FWD_RENDERER_NODE_INCLUDED_
#define FWD_RENDERER_NODE_INCLUDED_


#include <stdint.h>


namespace Simple_renderer {


struct Node
{
  float     wvp[16];
  uint32_t  diffuse_id;
  uint32_t  vbo_id;
  uint32_t  number_of_verts;
};



} // ns



#endif // inc guard