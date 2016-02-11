#ifndef MODEL_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B
#define MODEL_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B


#include <simple_renderer/vertex_buffer.hpp>
#include <cstddef>


#define SIZE_OF_MODEL_POOL 32


namespace World_data {


struct Model_pool
{
  std::size_t               id[SIZE_OF_MODEL_POOL];
  renderer::vertex_buffer   vbo[SIZE_OF_MODEL_POOL];
  
  const std::size_t         size = SIZE_OF_MODEL_POOL;
}; // struct


void
model_pool_init(Model_pool *pool);


} // ns


#endif // inc guard