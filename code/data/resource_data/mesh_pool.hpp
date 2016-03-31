#ifndef MESH_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B
#define MESH_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B


#include <graphics_api/mesh.hpp>
#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


#define SIZE_OF_MODEL_POOL 32


namespace Resource_data {


struct Mesh_pool
{
  uint32_t                    id[SIZE_OF_MODEL_POOL];
  char                        filename[SIZE_OF_MODEL_POOL * 256];
  Graphics_api::Mesh          mesh[SIZE_OF_MODEL_POOL];
  math::aabb                  aabb[SIZE_OF_MODEL_POOL];
  
  const uint32_t              capacity = SIZE_OF_MODEL_POOL;
}; // struct


void
mesh_pool_init(Mesh_pool *pool);


uint32_t
mesh_pool_push_new(Mesh_pool *pool,
                   const char *key,
                   const float *vertex_data,
                   const uint32_t number_of_vertices,
                   const uint32_t *index_data,
                   const uint32_t number_of_indices);




} // ns


#endif // inc guard