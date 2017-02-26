#include <transformations/rendering/material_renderer.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/context/material_data.hpp>
#include <data/memory/memory_data.hpp>
#include <math/transform/transform.hpp>


namespace Rendering {


uint32_t
material_renderer(const math::mat4 &view_mat,
                  const math::mat4 &proj_mat,
                  const math::vec3 eye_pos,
                  const float delta_time,
                  const float total_time,
                  const Data::Material_data *material_data,
                  const uint32_t camera_cull_mask,                  
                  const Data::Mesh_draw_call_data *mesh_renderer_data,
                  const Material_renderer::Draw_call *draw_calls,
                  const uint32_t number_of_draw_calls,
                  Ogl::Texture light_texture,
                  const size_t light_count)
{
  /*
    Sort the draw calls into runs.
    This is so we can bind a material, then batch draw calls.
  */
  struct Draw_run
  {
    uint32_t  material_id = 0;
    uint32_t          start_point = 0;
    uint32_t          size = 0;
  };
  
  // Its unlikely we will ever have more than 128 runs.
  // If we do we will increase it for the _next_ frame.
  LOG_TODO_ONCE("We did bust this limit!")
  static uint32_t runs_to_alloc_for = 128;
  
  Draw_run *runs = SCRATCH_ALIGNED_ALLOC(Draw_run, runs_to_alloc_for);
  memset(runs, 0, sizeof(Draw_run) * runs_to_alloc_for);
  
  uint32_t number_of_runs = 0;
  uint32_t number_of_draws = 0;
  {
    Draw_run *curr_run = &runs[0];
  
    // Push first material onto the list
    if(mesh_renderer_data->size)
    {
      *curr_run = Draw_run{};
      curr_run->material_id = mesh_renderer_data->field_material_id[0];
      curr_run->start_point = 0;
    }
  
    // Now calculate the runs
    // We start from one as we've already got that above.
    for(uint32_t i = 1; i < mesh_renderer_data->size; ++i)
    {
      if(curr_run->material_id != mesh_renderer_data->field_material_id[i])
      {
        curr_run->size = i - curr_run->start_point;
        ++number_of_runs;
        
        // Have we maxed out of runs?
        if(number_of_runs >= runs_to_alloc_for)
        {
          break;
        }
        
        curr_run = &runs[number_of_runs];
        
        *curr_run = Draw_run{};
        curr_run->material_id = mesh_renderer_data->field_material_id[i];
        curr_run->start_point = i;
      }
    }
    
    // Need to asign the size for the last run.
    if(number_of_runs)
    {
      curr_run->size = (mesh_renderer_data->size - curr_run->start_point);
      ++number_of_runs;
    }
    
    if(mesh_renderer_data->size && !number_of_runs)
    {
      LOG_TODO_ONCE("This is a hack to incase there is only 1 draw call, or 1 material")
      number_of_runs = 1;
      runs[0].size = mesh_renderer_data->size;
    }
  }
  
  // Do we need to increase the number of runs?
  if(number_of_runs >= runs_to_alloc_for)
  {
    --number_of_runs;
    LOG_INFO("Increasing memory for material runs.");
    runs_to_alloc_for = runs_to_alloc_for << 1;
  }
  
  // view proj matrix gets uploaded.
  const math::mat4 view_proj_mat = math::mat4_multiply(view_mat, proj_mat);
  
  /*
    Reset the render as other drawing operations might have left the GPU
    In a different state than we expect.
  */
  Material_renderer::reset();
  
  /*
    Loop through all the materials.
    And draw the run accordingly.
  */
  if(runs[0].size > 0 || number_of_runs > 0)
  {
    for(uint32_t r = 0; r < number_of_runs; ++r)
    {
      Material_renderer::Material material_to_render;
      
      Data::material_get_material(material_data,
                                  runs[r].material_id,
                                  &material_to_render);
      
      const size_t start = runs[r].start_point;
      const size_t count = runs[r].size;
      
      Ogl::error_clear();
      
      number_of_draws += Material_renderer::render(view_proj_mat,
                                                   eye_pos,
                                                   delta_time,
                                                   total_time,
                                                   &material_to_render,
                                                   camera_cull_mask,
                                                   &draw_calls[start],
                                                   count,
                                                   light_texture,
                                                   light_count);
    }
  }
  
  return number_of_draws;
}


} // ns
