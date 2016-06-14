#include <transformations/rendering/material_renderer.hpp>
#include <systems/renderer_material/material.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <data/global_data/memory_data.hpp>
#include <math/transform/transform.hpp>
#include <utilities/generic_id.hpp>


namespace Rendering {


uint32_t
material_renderer(const math::mat4 &view_mat,
                  const math::mat4 &proj_mat,
                  const Resource_data::Material_data *material_data,
                  const Resource_data::Mesh_data *mesh_data,
                  const World_data::Renderer_mesh_data *mesh_renderer_data,
                  const World_data::Transform_data *transform_data)
{
  /*
    Create draw calls the way the renderer wants them.
    The draw calls should already be in an optimised order, from here on in
    the only optimisation you can expect is the rendering remember the last thing bound,
    until its reset has been called.
  */
  Material_renderer::Draw_call *draw_calls = SCRATCH_ALIGNED_ALLOC(Material_renderer::Draw_call, mesh_renderer_data->size);
  {
    for(uint32_t i = 0; i < mesh_renderer_data->size; ++i)
    {
      // Draw call from the data.
      const World_data::Mesh_renderer_draw_call *draw_call_data = &mesh_renderer_data->property_draw_call[i];

      // No model? keep moving.
      if(!draw_call_data->model_id)
      {
        continue;
      }
      
      // Get the hardware mesh.
      // Possible extension. We could also process these based on how far away the camera is.
      Resource_data::mesh_data_get_property_mesh(mesh_data, draw_call_data->model_id, &draw_calls[i].mesh);
      
      // TODO: This is costly also we have the world matrix
      // Although it appears it might be a little wokey.
//      math::transform transform;
//      World_data::transform_data_get_property_transform(transform_data, mesh_renderer_data->entity_id[i], &transform);
//      
//      math::mat4 world = math::transform_get_world_matrix(transform);

      const float *world = draw_call_data->world_matrix;

      memcpy(&draw_calls[i], world, sizeof(float) * 16);
    }
  }

  /*
    Sort the draw calls into runs.
    This is so we can bind a material, then batch render a collection of draw calls.
  */
  struct Draw_run
  {
    util::generic_id material_id = 0;
    uint32_t start_point = 0;
    uint32_t size = 0;
  };
  
  // Its unlikely we will ever have more than 128 runs.
  // If we do we will increase it for the _next_ frame.
  static uint32_t runs_to_alloc_for = 128;
  
  Draw_run *runs = SCRATCH_ALIGNED_ALLOC(Draw_run, runs_to_alloc_for);
  uint32_t number_of_runs = 0;
  uint32_t number_of_draws = 0;
  {
    // Push first material onto the list
    if(mesh_renderer_data->size)
    {
      runs[0] = Draw_run{};
      runs[0].material_id = mesh_renderer_data->property_material_id[0];
      runs[0].start_point = 0;
    }
  
    // Now calculate the runs
    // We start from one as we've already got that above.
    for(uint32_t i = 1; i < mesh_renderer_data->size; ++i)
    {
      if(runs[number_of_runs].material_id != mesh_renderer_data->property_material_id[i])
      {
        runs[number_of_runs].size = i - runs[number_of_runs].start_point;
        ++number_of_runs;
        
        runs[number_of_runs] = Draw_run{};
        runs[number_of_runs].material_id = mesh_renderer_data->property_material_id[i];
        runs[number_of_runs].start_point = i;
      }
    }
    
    // Need to asign the size for the last run.
    if(number_of_runs)
    {
      runs[number_of_runs - 1].size = (mesh_renderer_data->size - runs[number_of_runs - 1].start_point);
      ++number_of_runs;
    }
  }
  
  // Do we need to increase the number of runs?
  if(number_of_runs >= runs_to_alloc_for)
  {
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
      Material_renderer::Material *material_to_render;
      Resource_data::material_data_get_property_material(material_data, runs[r].material_id, &material_to_render);
      
      const size_t start = runs[r].start_point;
      const size_t count = runs[r].size;
      
      Material_renderer::render(view_proj_mat,
                                material_to_render,
                                &draw_calls[start],
                                count);
    }
  }
  
  return number_of_draws;
}


} // ns