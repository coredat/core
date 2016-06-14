#include <transformations/rendering/material_renderer.hpp>
#include <systems/renderer_material/material.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <data/global_data/memory_data.hpp>
#include <math/transform/transform.hpp>

namespace Rendering {


void
material_renderer(const math::mat4 &view_mat,
                  const math::mat4 &proj_mat,
                  const Resource_data::Material_data *material_data,
                  const Resource_data::Mesh_data *mesh_data,
                  const World_data::Renderer_mesh_data *mesh_renderer_data,
                  const World_data::Transform_data *transform_data)
{
  const math::mat4 view_proj_mat = math::mat4_multiply(view_mat, proj_mat);
  
  Material_renderer::Draw_call *draw_calls = SCRATCH_ALIGNED_ALLOC(Material_renderer::Draw_call, mesh_renderer_data->size);

  uint32_t number_of_draw_calls = 0;
  
  for(uint32_t i = 0; i < mesh_renderer_data->size; ++i)
  {
    const World_data::Mesh_renderer_draw_call *draw_call_data = &mesh_renderer_data->property_draw_call[i];

    if(!draw_call_data->model_id)
    {
      ++number_of_draw_calls;
      continue;
    }

    Resource_data::mesh_data_get_property_mesh(mesh_data, draw_call_data->model_id, &draw_calls[number_of_draw_calls].mesh);
    
    if(draw_calls[number_of_draw_calls].mesh.vbo.vertex_buffer_id > 1000)
    {
      volatile int j = 0;
    }
    
    // Eh!!!! Don't we already have this?
    math::transform transform;
    World_data::transform_data_get_property_transform(transform_data, mesh_renderer_data->entity_id[i], &transform);
    
    math::mat4 world = math::transform_get_world_matrix(transform);
    memcpy(&draw_calls[number_of_draw_calls], &world, sizeof(float) * 16);
    
    if(draw_calls[number_of_draw_calls].mesh.vbo.vertex_buffer_id > 1000)
    {
      volatile int j = 0;
    }    
    
    ++number_of_draw_calls;
  }

  struct Draw_run
  {
    uint32_t material_id = 0;
    uint32_t start_point = 0;
    uint32_t size = 0;
  };
  
  Draw_run runs[128];
  uint32_t number_of_runs = 0;
  
  uint32_t count = 0;
  for(uint32_t i = 0; i < mesh_renderer_data->size; ++i)
  {
    if(mesh_renderer_data->property_material_id[i] == 0)
    {
      continue;
    }
    count++;
    // First run
    if(!number_of_runs)
    {
      runs[0] = Draw_run{};
      runs[0].material_id = mesh_renderer_data->property_material_id[i];
      runs[0].start_point = i;
      ++number_of_runs;
      continue;
    }
    
    if(mesh_renderer_data->property_material_id[i] != runs[number_of_runs - 1].material_id)
    {
      runs[number_of_runs - 1].size = (i - runs[number_of_runs - 1].start_point);
      ++number_of_runs;
      
      // New run
      runs[number_of_runs - 1].material_id = mesh_renderer_data->property_material_id[i];
      runs[number_of_runs - 1].start_point = i;
    }
  }
  
  if(number_of_runs)
  {
    runs[number_of_runs - 1].size = (mesh_renderer_data->size - runs[number_of_runs - 1].start_point);
  }
  
  ::Material_renderer::reset();
  if(runs[0].size > 0 || number_of_runs > 0)
  {
    for(uint32_t r = 0; r < number_of_runs; ++r)
    {
      ::Material_renderer::Material *mat;
      Resource_data::material_data_get_property_material(material_data, runs[r].material_id, &mat);
      
      const size_t start = runs[r].start_point;
      const size_t count = runs[r].size;
      
      ::Material_renderer::render(view_proj_mat, mat, &draw_calls[start], count);
    }
  }
}


} // ns