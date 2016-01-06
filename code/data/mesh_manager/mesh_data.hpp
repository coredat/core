#ifndef MESH_DATA_INCLUDED_A8754D41_F518_4378_A1D4_7B46EEE7387D
#define MESH_DATA_INCLUDED_A8754D41_F518_4378_A1D4_7B46EEE7387D


#include <cstddef>
#include <stdint.h>
#include <simple_renderer/vertex_buffer.hpp>
#include <simple_renderer/index_buffer.hpp>


#define MESH_BUFFER_SIZE 128


namespace Model {


struct Load_model
{
  const std::size_t id;
  const std::string file_name;
}; // struct





} // ns


namespace Data {


struct Mesh
{
  uint32_t                  id[MESH_BUFFER_SIZE];
  bool                      avail[MESH_BUFFER_SIZE]; // temp
  renderer::vertex_buffer   vbo[MESH_BUFFER_SIZE];
  renderer::index_buffer    ibo[MESH_BUFFER_SIZE];
  
  const std::size_t         size = MESH_BUFFER_SIZE;
};


void          mesh_init_data(Mesh *data, const std::size_t number_of_meshes);
bool          mesh_is_valid(const Mesh *data, const std::size_t mesh_id);
uint32_t      mesh_get_api_id(const Mesh *data, const std::size_t mesh_id);
std::size_t   mesh_add_new(Mesh *data, const std::size_t number_of_meshes, const char* filepath, const uint32_t mesh_id);
std::size_t   mesh_find_index(const Mesh *data, const std::size_t number_of_meshes, const uint32_t id);


} // ns


#endif // inc guard