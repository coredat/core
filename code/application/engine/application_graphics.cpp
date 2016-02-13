#include "application_graphics.hpp"
#include <core/interface/entity.hpp>
#include <data/data.hpp>
#include <systems/transform/transform.hpp>
#include <renderer/simple_renderer/simple_renderer_node.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <math/math.hpp>


namespace Application {


void
graphics_think(World_data::World *world,
               Simple_renderer::Node nodes[],
               const std::size_t size_of_node_pool)
{
  renderer::clear();

  // Get active camera and generate a projection matrix.
  const auto cam = World_data::camera_pool_get_properties_for_priority(world->camera_pool, 1);
  const math::mat4 proj = math::mat4_projection(cam.viewport_width, cam.viewport_height, cam.near_plane, cam.far_plane, cam.fov);
  
  // Get entity's transform so we can generate a view.
  math::mat4 view = math::mat4_zero();
  {
    const auto id = World_data::camera_pool_get_entity_id_for_priority(world->camera_pool, 1);

    // If we cant find the camera we'll just make a dummy orbit one for the time.
    // This is good for debugging.
    if (id != Core::Entity_id_util::invalid_id())
    {
      Core::Entity ent;
      World_data::world_find_entity(world, &ent, id);

      const math::transform camera_transform = ent.get_transform();

      math::vec3 cam_fwd;
      Transform::get_fwd_vec(&camera_transform, &cam_fwd);

      math::vec3 cam_up;
      Transform::get_up_vec(&camera_transform, &cam_up);

      view = math::mat4_lookat(camera_transform.position, math::vec3_add(camera_transform.position, cam_fwd), cam_up);
    }
    else
    {
      static float time = 4;
      time += 0.005f;

      const float x = math::sin(time) * 9;
      const float z = math::cos(time) * 9;

      view = math::mat4_lookat(math::vec3_init(x, 5, z), math::vec3_zero(), math::vec3_init(0, 1, 0));

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
  for (std::size_t i = 0; i < world->entity_pool->size; ++i)
  {
    nodes[i].vbo = world->model_pool->vbo[world->entity_pool->model[i]];
    nodes[i].diffuse_id = World_data::texture_pool_find(world->texture_pool, world->entity_pool->texture[i])->texture_id;
  }

  Simple_renderer::render_nodes_fullbright(nodes, size_of_node_pool);
  //Simple_renderer::render_nodes_directional_light(renderer_nodes.data(), renderer_nodes.size(), &eye_pos[0]);

  math::mat4 wvp = math::mat4_multiply(math::mat4_id(), view, proj);
  Debug_line_renderer::render(math::mat4_get_data(wvp));
}


} // ns