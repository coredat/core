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
mesh_add_new(Mesh *data, const std::size_t number_of_meshes, const char *filepath)
{
  // Could just memset?
  for(std::size_t i = 0; i < number_of_meshes; ++i)
  {
    if(data->avail[i] == true)
    {
      const util::obj_model model = util::load_obj(filepath);
      const util::gl_mesh mesh    = util::convert_to_open_gl_mesh(model.meshes.front());
      
      const renderer::vertex_buffer vbo(mesh.mesh_data);
      
      data->avail[i] = false;
      data->vbo[i] = vbo;
      
      return 0;
    }
  }
  
  return 0;
}


} // ns