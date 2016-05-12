#include <core/renderer/mesh_renderer.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/color/color.hpp>
#include <core/color/color_utils.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/transform/transform.hpp>

#include <graphics_api/clear.hpp>
#include <graphics_api/initialize.hpp>
#include <graphics_api/mesh.hpp>

#include <renderer/simple_renderer/simple_renderer_node.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>

#include <data/global_data/resource_data.hpp>

#include <systems/transform/transformations.hpp>
#include <data/world_data/world_data.hpp>
#include <math/math.hpp>
#include <vector>



namespace Core {


Mesh_renderer::Mesh_renderer()
{
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
}


void
Mesh_renderer::render()
{
//  const uint32_t peer = 0;
//  World_data::World *world = World_data::get_world();
//
//  static std::vector<Simple_renderer::Node> nodes;
//  nodes.resize(world->entity_pool->size);
//  const uint32_t size_of_node_pool = nodes.size();
//
//  // Get active camera and generate a projection matrix.
//  // TODO: Need to pass in the environment height and width into this function.
//  const auto cam = World_data::camera_pool_get_properties_for_priority(world->camera_pool, peer, 0);
//  
//  math::mat4 proj;
//  
//  if(cam.type == Core::Camera_type::orthographic)
//  {
//    proj = math::mat4_orthographic(cam.viewport_width,
//                                   cam.viewport_height,
//                                   cam.near_plane,
//                                   cam.far_plane);
//  }
//  else
//  {
//    proj = math::mat4_projection(cam.viewport_width,
//                                 cam.viewport_height,
//                                 cam.near_plane,
//                                 cam.far_plane,
//                                 cam.fov);
//}
//  
//  const Core::Color clear_color(cam.clear_color);
//  const float red = Core::Color_utils::get_red_f(clear_color);
//  const float green = Core::Color_utils::get_green_f(clear_color);
//  const float blue = Core::Color_utils::get_blue_f(clear_color);
//  
//  Graphics_api::clear_color_set(red, green, blue);
//  
//  const uint32_t flags = Graphics_api::Clear_flag::color | Graphics_api::Clear_flag::depth;
//  Graphics_api::clear(flags);
//  
//  // Get entity's transform so we can generate a view.
//  math::mat4 view = math::mat4_zero();
//  {
//    const auto id = World_data::camera_pool_get_entity_id_for_priority(world->camera_pool,
//                                                                       peer,
//                                                                       0);
//    
//    // If we cant find the camera we'll just make a dummy orbit one for the time.
//    // This is good for debugging.
//    if (id != util::generic_id_invalid())
//    //if(false) // debug cam route
//    {
//      size_t ent_index;
//      World_data::entity_pool_find_index(world->entity_pool, id, &ent_index);
//
//      const math::transform trans = world->entity_pool->transform[ent_index];
//      const Core::Transform cam_transform(trans.position, trans.scale, trans.rotation);
//      
//      view = math::mat4_lookat(cam_transform.get_position(),
//                               math::vec3_add(cam_transform.get_position(), cam_transform.get_forward()),
//                               cam_transform.get_up());
//    }
//    else
//    {
//      static float time = 4;
//      time += 0.005f;
//
//      const float x = math::sin(time) * 9;
//      const float z = math::cos(time) * 9;
//
//      const math::vec3 eye_pos = math::vec3_init(x, 5.f, z);
//      const math::vec3 look_at = math::vec3_zero();
//      const math::vec3 up      = math::vec3_init(0.f, 1.f, 0.f);
//      
//      view = math::mat4_lookat(eye_pos, look_at, up);
//    }
//  }
//
//  const math::mat4 view_proj = math::mat4_multiply(view, proj);
//
//  ::Transform::transforms_to_wvp_mats(world->entity_pool->transform,
//                                      world->entity_pool->size,
//                                      view_proj,
//                                      nodes[0].wvp,
//                                      size_of_node_pool,
//                                      sizeof(Simple_renderer::Node));
//
//  ::Transform::transforms_to_world_mats(world->entity_pool->transform,
//                                        world->entity_pool->size,
//                                        nodes[0].world_mat,
//                                        size_of_node_pool,
//                                        sizeof(Simple_renderer::Node));
//  
//  // Texture/vbo info
//  for (uint32_t i = 0; i < size_of_node_pool; ++i)
//  {
//    const uint32_t mesh_id    = world->entity_pool->model[i];
//    const uint32_t texture_id = world->entity_pool->texture[i];
//    
//    if(mesh_id && texture_id)
//    {
//      Resource_data::Resources *resources = Resource_data::get_resources();
//      
//      Graphics_api::Mesh get_mesh = Resource_data::mesh_pool_find(resources->mesh_pool, mesh_id);
//      Ogl::Texture get_texture    = Resource_data::texture_pool_find(resources->texture_pool, texture_id);
//      
//      nodes[i].vbo     = get_mesh.vbo;
//      nodes[i].diffuse = get_texture;
//    }
//  }
//  
//  Simple_renderer::render_nodes_fullbright(nodes.data(), size_of_node_pool);
//  //Simple_renderer::render_nodes_directional_light(nodes, size_of_node_pool);
//
//  math::mat4 wvp = math::mat4_multiply(math::mat4_id(), view, proj);
//  Debug_line_renderer::render(math::mat4_get_data(wvp));
}


} // ns