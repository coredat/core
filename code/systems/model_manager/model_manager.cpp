#include "model_manager.hpp"
#include <utils/obj_model_loader.hpp>
#include <assert.h>
#include <algorithm>


namespace Model {


void
model_load(const Load_model models_to_load[],
           const std::size_t number_of_models_to_load,
           Ogl::Vertex_buffer output_vbo[],
           const std::size_t size_of_output_pool)
{
  const std::size_t number_to_load = std::min(number_of_models_to_load, size_of_output_pool);
  
  for(std::size_t i = 0; i < number_to_load; ++i)
  {
    const auto &model_to_load = models_to_load[i];
    
    // We don't have space for this.
    if(model_to_load.id > size_of_output_pool)
    {
      assert(false);
      continue;
    }
    
    const util::obj_model model(util::load_obj(model_to_load.file_name));
    
    // No data.
    if(model.meshes.empty())
    {
      assert(false);
      continue;
    }
    
    const util::gl_mesh mesh(util::convert_to_open_gl_mesh(model.meshes.front()));
    
    Ogl::Vertex_buffer vbo;
    Ogl::vertex_buffer_load(&vbo, mesh.mesh_data.data(), sizeof(float) * mesh.mesh_data.size(), mesh.mesh_data.size(), false);
    
    output_vbo[i] = vbo;
  }
}


void
find_model_data(Ogl::Vertex_buffer output_vbo[],
                const std::size_t size_of_output,
                const uint32_t input_ids[],
                const Ogl::Vertex_buffer input_vbo[],
                const std::size_t size_of_input)
{
  for(std::size_t i = 0; i < size_of_output; ++i)
  {
    output_vbo[i] = input_vbo[input_ids[i]];
  }
}


} // ns