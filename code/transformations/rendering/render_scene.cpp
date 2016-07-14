#include <transformations/rendering/render_scene.hpp>
#include <data/global_data/memory_data.hpp>
#include <systems/renderer_material/material.hpp>
#include <systems/renderer_post/post_process.hpp>
#include <systems/renderer_post/post_shader.hpp>

#include <transformations/rendering/material_renderer.hpp>
#include <core/color/color_utils.hpp>
#include <graphics_api/clear.hpp>


namespace Rendering {


/*!
  This will call the various renderers.
*/
void
render_main_scene(const float delta_time,
                  const float total_time,
                  const World_data::Renderer_mesh_data   *mesh_renderer_data,
                  const Resource_data::Material_data     *material_data,
                  const Resource_data::Post_process_data *post_data,
                  const Camera_utils::Cam_run            cam_runs[],
                  const uint32_t                         number_of_cam_runs,
                  const Material_renderer::Draw_call     draw_calls[],
                  const uint32_t                         number_of_draws,
                  uint32_t *out_draw_calls)
{
  // Stats to gather
  uint32_t number_of_draw_calls = 0;
  
  
  for(uint32_t c = 0; c < number_of_cam_runs; ++c)
  {
    const Camera_utils::Cam_run *cam = &cam_runs[c];
    
    // Set the target if we have one.
    if(Ogl::frame_buffer_is_valid(&cam->fbo))
    {
      Ogl::frame_buffer_bind(&cam->fbo);
      
      LOG_TODO_ONCE("Move this to graphcis api somewhere.");
      
      const GLsizei width = cam->fbo.color_buffer[0].width;
      const GLsizei height = cam->fbo.color_buffer[0].height;
      
      glViewport(0, 0, width, height);
    }
    else
    {
      Ogl::frame_buffer_unbind();
      
      LOG_TODO_ONCE("Move this to graphcis api somewhere.");
      
      const GLsizei width = cam->width;
      const GLsizei height = cam->height;
      
      glViewport(0, 0, width, height);
    }
    
    // Clear the target
    {
      if(cam->clear_flags)
      {    
        const Core::Color clear_color(cam->clear_color);
        
        const float red   = Core::Color_utils::get_red_f(clear_color);
        const float green = Core::Color_utils::get_green_f(clear_color);
        const float blue  = Core::Color_utils::get_blue_f(clear_color);
        
        Graphics_api::clear_color_set(red, green, blue);
        Graphics_api::clear(cam->clear_flags);
      }
    }

    // Material Renderer
    if(!cam->post_process_id)
    {
      number_of_draw_calls += Rendering::material_renderer(cam->view,
                                                           cam->proj,
                                                           delta_time,
                                                           total_time,
                                                           material_data,
                                                           cam->cull_mask,
                                                           mesh_renderer_data,
                                                           draw_calls,
                                                           mesh_renderer_data->size);
    }
    else
    // Post process rendering
    {    
      // Get post process details
      Post_renderer::Post_shader *post_shd;
      Resource_data::post_process_data_get_property_post_shader(post_data,
                                                                cam->post_process_id,
                                                                &post_shd);
      
      // Add some extra param to the shd.
      post_shd->delta_time = delta_time;
      post_shd->current_time = total_time;
      
      
      // Render
      Post_renderer::render(post_shd);
      
      ++number_of_draw_calls;
    }
  }

  // Return some stats
  *out_draw_calls += number_of_draw_calls;
}


} // ns