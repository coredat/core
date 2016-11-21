#include <transformations/rendering/render_scene.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/post_process_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world_data.hpp>
#include <systems/renderer_material/material.hpp>
#include <systems/renderer_post/post_process.hpp>
#include <systems/renderer_post/post_shader.hpp>
#include <systems/renderer_text/text_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
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
                  const uint32_t viewport_x,
                  const uint32_t viewport_y,                  
                  const Data::World                      *world,
                  const Data::Material_data     *material_data,
                  const Data::Post_process_data *post_data,
                  const Camera_utils::Cam_run            cam_runs[],
                  const uint32_t                         number_of_cam_runs,
                  const Material_renderer::Draw_call     draw_calls[],
                  const uint32_t                         number_of_draws,
                  uint32_t *out_draw_calls)
{
  LOG_TODO_ONCE("Pass the data not the containers here, This makes it difficult to use for other things Phil!")

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
      
      const GLsizei width = cam->fbo.color_buffer[0].width; // viewport_x ?
      const GLsizei height = cam->fbo.color_buffer[0].height; // viewport_y ?
      
      glViewport(0, 0, width, height);
    }
    else
    {
      Ogl::frame_buffer_unbind();
      
      LOG_TODO_ONCE("Move this to graphcis api somewhere.");
      
      const GLsizei width = viewport_x;// cam->width;
      const GLsizei height = viewport_y;//cam->height;
      
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
                                                           cam->eye_pos,
                                                           delta_time,
                                                           total_time,
                                                           material_data,
                                                           cam->cull_mask,
                                                           world->mesh_data,
                                                           draw_calls,
                                                           world->mesh_data->size);
      

      const math::mat4 view_proj = math::mat4_multiply(cam->view, cam->proj);
    
      if(c == 2)
      {
        LOG_TODO_ONCE("Complete hack! Draw calls need to be revisited, will deal with this then.");
      
        number_of_draw_calls += Text_renderer::render(view_proj,
                                                      cam->cull_mask,
                                                      world->text_data->field_draw_call,
                                                      world->text_data->size);
      }
      
      math::mat4 wvp = math::mat4_multiply(cam->view, cam->proj);
      
      ::Debug_line_renderer::Line_node node;
      
      const float color[3] = {1,0,0};
      memcpy(node.color, color, sizeof(color));
      
      const float pos[3] = {0,0,0};
      memcpy(node.position_from, pos, sizeof(pos));
      
      const float pos2[3] = {3,3,3};
      memcpy(node.position_to, pos2, sizeof(pos2));
      
      ::Debug_line_renderer::render(math::mat4_get_data(wvp));
    }
    else
    // Post process rendering
    {
      // Get post process details
      Post_renderer::Post_shader post_shd;
      Data::post_process_get_post_shader(post_data,
                                          cam->post_process_id,
                                          &post_shd);
      
      // Add some extra param to the shd.
      post_shd.delta_time = delta_time;
      post_shd.current_time = total_time;
      
      
      // Render
      Post_renderer::render(&post_shd);
      
      ++number_of_draw_calls;
    }
  }

  // Return some stats
  *out_draw_calls += number_of_draw_calls;
}


} // ns