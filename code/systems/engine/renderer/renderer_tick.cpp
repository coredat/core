#include "renderer_tick.hpp"

#include <data/world_data.hpp>
#include <data/context_data.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context/mesh_data.hpp>
#include <data/world_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/camera_data.hpp>
#include <data/world/light_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/context/texture_data.hpp>

#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/simple_renderer/simple_renderer_node.hpp>
#include <systems/renderer_aabb/renderer_aabb.hpp>
#include <systems/renderer_text/text_renderer.hpp>
#include <systems/renderer_post/post_process.hpp>
#include <systems/renderer_material/material_renderer.hpp>

#include <graphics_api/texture.hpp>

#include <transformations/camera/cam_priorities.hpp>
#include <transformations/rendering/render_scene.hpp>

#include <graphics_api/context.hpp>
#include <graphics_api/texture_desc.hpp>

// Remove
#include <core/transform/transform.hpp>


namespace
{
  Graphics_api::Context gfx_context;
}


namespace Engine {
namespace Renderer_tick {


void
initialize()
{
//  Graphics_api::command_buffer_create(&m_impl->graphcis_command_buffer, 1 << 17);
  
//  Simple_renderer::initialize(); // TODO: This can be removed I think, largely superceded by mat renderer
  Debug_line_renderer::initialize();
  Aabb_renderer::initialize();
  
  ::Text_renderer::initialize();
  ::Material_renderer::initialize();
  ::Post_renderer::initialize();
}


void
think(std::shared_ptr<Data::World> world,
      std::shared_ptr<Data::Context> resources,
      const float dt,
      const float running_time,
      const uint32_t width,
      const uint32_t height,
      Tick_information *out_tick_info)
{
  static Ogl::Texture light_texture;
  static Graphics_api::Texture_desc light_texture_desc;
  static size_t light_count = 0;

  /*
    Lighting
  */
  {
    static int loaded = 0;
    
    if(!loaded)
    {
      loaded = 1;
      const uint32_t max_lights = 25;
      const uint32_t components_per_light = 12;
      Ogl::texture_create_1d(&light_texture,
                             max_lights * components_per_light,
                             GL_RGBA32F,
                             &loaded);
      
      light_texture_desc.pixel_format = Graphics_api::Pixel_format::rgba32f;
      light_texture_desc.width = max_lights * components_per_light;
      
      gfx_context.texture_create(&light_texture_desc, &loaded);
      gfx_context.exec();
    }
  }
  
  // Update light texture
  {
    auto light_data = world->light_data;
  
    Ogl::texture_update_texture_1d(&light_texture,
                                   0,
                                   light_texture.width,
                                   light_data->field_light);
    
    Graphics_api::Envelope env{0,0,0,light_texture.width, light_texture.width, light_texture.width};
    
    gfx_context.texture_update(
      light_texture_desc.internal_handle,
      &env,
      light_data->field_light);
    
    light_texture.texture_id = light_texture_desc.platform_handle;
    
    light_count = light_data->size;
  }

  /*
    Camera Runs
    --
    For each camera we need a to create a camera run.
    Will will render all the things it is interested in.
    
    TODO
    --
    Can we async this?
  */
  auto cam_data = world->camera_data;

  Data::data_lock(cam_data);
  uint32_t number_of_cam_runs = 0;
  
  Camera_utils::Cam_run *cam_runs = SCRATCH_ALIGNED_ALLOC(Camera_utils::Cam_run, cam_data->size);
  memset(cam_runs, 0, sizeof(Camera_utils::Cam_run) * cam_data->size);
  {
    number_of_cam_runs = cam_data->size;
    
    // Generate cam_run data
    {
      Core::Transform *cam_transforms = SCRATCH_ALIGNED_ALLOC(Core::Transform, cam_data->size);
      
      Camera_utils::get_camera_transforms(world->transform,
                                          cam_data->keys,
                                          cam_transforms,
                                          cam_data->size);
      
      Camera_utils::calculate_camera_runs(cam_data,
                                          resources->texture_data,
                                          cam_transforms,
                                          cam_runs,
                                          cam_data->size);
    }
  }
  
  Data::data_unlock(cam_data);
  
  
  /*
    Generate the Draw call list
    --
    This list is every draw call that needs to be rendered.
    
    TODO
    --
    Can we async this?
  */
  const size_t draw_call_count = world->mesh_data->size;
  ::Material_renderer::Draw_call *draw_calls = SCRATCH_ALIGNED_ALLOC(::Material_renderer::Draw_call, draw_call_count);
  memset(draw_calls, 0, sizeof(::Material_renderer::Draw_call) * draw_call_count);
  {
    for(uint32_t i = 0; i < world->mesh_data->size; ++i)
    {
      // Draw call from the data.
      const Data::Mesh_renderer_draw_call *draw_call_data = &world->mesh_data->field_draw_call[i];

      // No model? keep moving.
      if(!draw_call_data->model_id)
      {
        continue;
      }
      
      // Get the hardware mesh.
      // Possible extension. We could also process these based on how far away the camera is.
      Data::mesh_get_mesh(resources->mesh_data, draw_call_data->model_id, &draw_calls[i].mesh);
      
      const float *world_mat = draw_call_data->world_matrix;

      memcpy(&draw_calls[i], world_mat, sizeof(float) * 16);
      
      // Get cull mask.
      // This isn't particularly nice. We should already have this data to save us looking for it.
      const util::generic_id entity_id = world->mesh_data->keys[i];
      Data::entity_get_tags(world->entity, entity_id, &draw_calls[i].cull_mask);
    }
  }
  
  
  // Render before as it will use the camera matrix from the wrong frame otherwise.
  #ifdef CORE_DEBUG_MENU
//  world->scene->Render(&debug_renderer);
  #endif
  
  
  /*
    Render the world
    --
    Takes the camera, and draw calls and renders the world accordingly.
  */
  uint32_t number_of_draw_calls = 0;
  Rendering::render_main_scene(dt,
                               running_time,
                               width, // context data!
                               height,
                               world.get(),
                               resources->material_data,
                               resources->post_process_data,
                               cam_runs,
                               number_of_cam_runs,
                               draw_calls,
                               world->mesh_data->size,
                               light_texture,
                               light_count,
                               &number_of_draw_calls);
  
  LOG_TODO_ONCE("Scratch code for text rendering");
  
  for(uint32_t i = 0; i < number_of_cam_runs; ++i)
  {
    auto cam = &cam_runs[i];
    
    if(cam->post_process_id)
    {
      continue;
    }
//
//    const math::mat4 scale     = math::mat4_scale(math::vec3_init(1.f));
//    const math::mat4 world     = math::mat4_multiply(math::mat4_id(), scale);
//    const math::mat4 view_proj = math::mat4_multiply(cam_runs[i].view, cam_runs[i].proj);
//
//    const GLsizei width  = m_impl->context->get_width(); //cam->fbo.color_buffer[0].width; // viewport_x ?
//    const GLsizei height = m_impl->context->get_height(); //cam->fbo.color_buffer[0].height; // viewport_y ?
//    
//    glViewport(0, 0, width, height);
//
//    ::Text_renderer::render(view_proj, m_impl->world_data->text_data->property_draw_call, m_impl->world_data->text_data->size);
  }
  
  out_tick_info->number_of_draw_calls = number_of_draw_calls;
  out_tick_info->camera_runs = number_of_cam_runs;
  
  /*
   Testing
  */
//  auto buf = &m_impl->graphcis_command_buffer;
//  Graphics_api::command_buffer_execute(buf);
}


void
de_initialize()
{
}


} // ns
} // ns