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
graphics_think(Data::World *world,
               Simple_renderer::Node nodes[],
               const std::size_t size_of_node_pool)

{
  renderer::clear();


  // Get active camera and generate a projection matrix.
  auto cam = world->camera_pool->camera[0];
  const math::mat4 proj = math::mat4_projection(cam.viewport_width, cam.viewport_height, cam.near_plane, cam.far_plane, cam.fov);
  
  // Get entity's transform so we can generate a view.
  math::mat4 view = math::mat4_zero();
  {
    Core::Entity ent;
    Data::world_find_entity(world, &ent, world->camera_pool->entity_id[0]);
    
    const math::transform camera_transform = ent.get_transform();
    
    math::vec3 cam_fwd;
    Transform::get_fwd_vec(&camera_transform, &cam_fwd);
    
    math::vec3 cam_up;
    Transform::get_up_vec(&camera_transform, &cam_up);
    
    view = math::mat4_lookat(camera_transform.position, math::vec3_add(camera_transform.position, cam_fwd), cam_up);
  }
  

  static float time = 4;
  //time += delta_time / 2;

  const float x = math::sin(time) * 9;
  const float z = math::cos(time) * 9;

//  const math::mat4 view = math::mat4_lookat(math::vec3_init(x, 3.4f, z),
//    math::vec3_zero(),
//    math::vec3_init(0, 1, 0));

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
    nodes[i].diffuse_id = Data::texture_pool_find(world->texture_pool, world->entity_pool->texture[i])->texture_id;
  }

  Simple_renderer::render_nodes_fullbright(nodes, size_of_node_pool);
  //Simple_renderer::render_nodes_directional_light(renderer_nodes.data(), renderer_nodes.size(), &eye_pos[0]);

  math::mat4 wvp = math::mat4_multiply(math::mat4_id(), view, proj);
  Debug_line_renderer::render(math::mat4_get_data(wvp));
}


} // ns