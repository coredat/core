#ifndef MESH_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B
#define MESH_POOL_INCLUDED_B5BA64BB_268D_4533_8EE5_509ECFD72F4B


#include <graphics_api/mesh.hpp> // Fwd dec
#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


#define SIZE_OF_MODEL_POOL 32
#define MESH_POOL_MAX_FILEPATH_SIZE 256


namespace Resource_data {


struct Mesh_pool
{
  uint32_t                    id[SIZE_OF_MODEL_POOL];
  char                        filename[SIZE_OF_MODEL_POOL * MESH_POOL_MAX_FILEPATH_SIZE];
  Graphics_api::Mesh          mesh[SIZE_OF_MODEL_POOL];
  math::aabb                  aabb[SIZE_OF_MODEL_POOL];
  
  const uint32_t              capacity = SIZE_OF_MODEL_POOL;
}; // struct


/*
  Initialize the mesh pool
*/
void
mesh_pool_init(Mesh_pool *pool);


/*!
  Find the mesh for the given id.
*/
Graphics_api::Mesh
mesh_pool_find(const Mesh_pool *pool, const uint32_t id);


/*!
 
*/
bool
mesh_pool_find_id_by_name(const Mesh_pool *pool,
                          const char *name,
                          uint32_t *result);

/*!
  Push a new mesh via data into the pool.
*/
uint32_t
mesh_pool_push_new(Mesh_pool *pool,
                   const char *key,
                   const float *positions,
                   const uint32_t size_of_positions,
                   const float *normals,
                   const uint32_t size_of_normals,
                   const float *uvs,
                   const uint32_t size_of_uvs,
                   const uint32_t number_of_vertices,
                   const uint32_t *index_data,
                   const uint32_t number_of_indices);


} // ns


#endif // inc guard