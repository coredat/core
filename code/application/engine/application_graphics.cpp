#include "application_graphics.hpp"
#include <core/interface/entity.hpp>
#include <data/data.hpp>
#include <systems/transform/transformations.hpp>
#include <renderer/simple_renderer/simple_renderer_node.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <math/math.hpp>


namespace Application {


void
graphics_think(World_data::World *world,
               Simple_renderer::Node nodes[],
               const std::uint32_t size_of_node_pool,
               const std::uint32_t peer)
{
  renderer::clear();

  // Get active camera and generate a projection matrix.
  const auto cam = World_data::camera_pool_get_properties_for_priority(world->camera_pool, peer, 1);
  const math::mat4 proj = math::mat4_projection(cam.viewport_width,
                                                cam.viewport_height,
                                                cam.near_plane,
                                                cam.far_plane,
                                                cam.fov);
  
  // Get entity's transform so we can generate a view.
  math::mat4 view = math::mat4_zero();
  {
    const auto id = World_data::camera_pool_get_entity_id_for_priority(world->camera_pool, peer, 1);

    // If we cant find the camera we'll just make a dummy orbit one for the time.
    // This is good for debugging.
    if (id != Core::Entity_id_util::invalid_id())
    //if(false) // debug cam route
    {
      Core::Entity ent;
      World_data::world_find_entity(world, &ent, id);

      const Core::Transform cam_transform = ent.get_transform();
      
      view = math::mat4_lookat(cam_transform.get_position(), math::vec3_add(cam_transform.get_position(), cam_transform.get_forward()), cam_transform.get_up());
    }
    else
    {
      static float time = 4;
      //time += 0.005f;

      const float x = math::sin(time) * 9;
      const float z = math::cos(time) * 9;

      const math::vec3 eye_pos = math::vec3_init(x, 5, z);
      const math::vec3 look_at = math::vec3_zero();
      const math::vec3 up      = math::vec3_init(0.f, 1.f, 0.f);
      
      view = math::mat4_lookat(eye_pos, look_at, up);
    }
  }

  const math::mat4 view_proj = math::mat4_multiply(view, proj); // *hurt* camaera or such.
  Transform::transforms_to_wvp_mats(world->entity_pool->transform,
                                    world->entity_pool->size,
                                    view_proj,
                                    nodes[0].wvp,
                                    size_of_node_pool,
                                    sizeof(Simple_renderer::Node));


  Transform::transforms_to_world_mats(world->entity_pool->transform,
                                      world->entity_pool->size,
                                      nodes[0].world_mat,
                                      size_of_node_pool,
                                      sizeof(Simple_renderer::Node));

  // Texture/vbo info
  for (uint32_t i = 0; i < size_of_node_pool; ++i)
  {
    nodes[i].vbo     = world->model_pool->vbo[world->entity_pool->model[i]];
    nodes[i].diffuse = World_data::texture_pool_find(world->texture_pool, world->entity_pool->texture[i]);
  }
  
  //Simple_renderer::render_nodes_fullbright(nodes, size_of_node_pool);
  Simple_renderer::render_nodes_directional_light(nodes, size_of_node_pool);

  math::mat4 wvp = math::mat4_multiply(math::mat4_id(), view, proj);
  Debug_line_renderer::render(math::mat4_get_data(wvp));
}


} // ns