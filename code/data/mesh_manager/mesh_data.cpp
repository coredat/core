#include "mesh_data.hpp"
#include <utils/obj_model_loader.hpp>
#include <assert.h>



namespace Data {


void
mesh_init_data(Mesh *data, const std::size_t number_of_meshes)
{
  // Could just memset?
  for(std::size_t i = 0; i < number_of_meshes; ++i)
  {
    data->avail[i] = true;
  }
}


bool
mesh_is_valid(const Mesh *data, const std::size_t texture_id)
{
  // DANGER!
  assert(data);
  return data->vbo[texture_id].is_valid() > 0;
}


std::size_t
mesh_add_new(Mesh *data, const std::size_t number_of_meshes, const char *filepath, const uint32_t mesh_id)
{
  // Could just memset?
  for(std::size_t i = 0; i < number_of_meshes; ++i)
  {
    if(data->avail[i] == true)
    {
      const util::obj_model model = util::load_obj(filepath);
      const util::gl_mesh mesh    = util::convert_to_open_gl_mesh(model.meshes.front());
      
      const renderer::vertex_buffer vbo(mesh.mesh_data);
      
      data->id[i]     = mesh_id;
      data->avail[i]  = false;
      data->vbo[i]    = vbo;
      
      return 0;
    }
  }
  
  return 0;
}


std::size_t
mesh_find_index(const Mesh *data, const std::size_t number_of_meshes, const uint32_t id)
{
  // Param
  assert(data && number_of_meshes);

  for(std::size_t i = 0; i < number_of_meshes; ++i)
  {
    if(data->id[i] == id)
    {
      return i;
    }
  }
  
  return 0; // TODO: urgh can't be 0.
}


} // ns