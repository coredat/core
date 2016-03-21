#ifndef MODEL_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B
#define MODEL_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B


#include <graphics_api/ogl/ogl_vertex_buffer.hpp>
#include <math/geometry/geometry.hpp>
#include <stdint.h>


#define SIZE_OF_MODEL_POOL 32


namespace World_data {


struct Model_pool
{
  uint32_t                  id[SIZE_OF_MODEL_POOL];
//  renderer::vertex_buffer   vbo[SIZE_OF_MODEL_POOL];
  Ogl::Vertex_buffer        vbo[SIZE_OF_MODEL_POOL];
  math::aabb                aabb[SIZE_OF_MODEL_POOL];
  
  const uint32_t              size = SIZE_OF_MODEL_POOL;
}; // struct


void
model_pool_init(Model_pool *pool);


} // ns


#endif // inc guard