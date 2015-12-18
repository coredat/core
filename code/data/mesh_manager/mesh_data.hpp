#ifndef MESH_DATA_INCLUDED_
#define MESH_DATA_INCLUDED_


#include <cstddef>
#include <stdint.h>
#include <simple_renderer/vertex_buffer.hpp>
#include <simple_renderer/index_buffer.hpp>


#define MESH_BUFFER_SIZE 128


namespace Data {


struct Mesh
{
  bool                    avail[MESH_BUFFER_SIZE]; // temp
  renderer::vertex_buffer vbo[MESH_BUFFER_SIZE];
  renderer::index_buffer  ibo[MESH_BUFFER_SIZE];
};


void          mesh_init_data(Mesh *data, const std::size_t number_of_meshes);
bool          mesh_is_valid(const Mesh *data, const std::size_t mesh_id);
uint32_t      mesh_get_api_id(const Mesh *data, const std::size_t mesh_id);
std::size_t   mesh_add_new(Mesh *data, const std::size_t number_of_meshes, const char* filepath);


} // ns


#endif // inc guard