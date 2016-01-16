#include "application_graphics.hpp"
#include <data/entity_pool.hpp>
#include <data/texture_pool.hpp>
#include <data/model_pool.hpp>
#include <systems/transform/transform.hpp>
#include <renderer/simple_renderer/simple_renderer_node.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <math/math.hpp>


namespace Application {


void
graphics_think(const Data::Entity_pool *entities,
  const Data::Texture_pool *texture_pool,
  const Data::Model_pool *model_pool,
  Simple_renderer::Node nodes[],
  const std::size_t size_of_node_pool)
{
  renderer::clear();

  static const math::mat4  proj = math::mat4_projection(800, 480, 0.1, 1000, math::quart_tau() * 0.6f);

  static float time = 4;
  //time += delta_time / 2;

  const float x = math::sin(time) * 9;
  const float z = math::cos(time) * 9;

  const math::mat4 view = math::mat4_lookat(math::vec3_init(x, 3.4f, z),
    math::vec3_zero(),
    math::vec3_init(0, 1, 0));

  const math::mat4 view_proj = math::mat4_multiply(view, proj); // *hurt* camaera or such.
  Transform::transforms_to_wvp_mats(entities->transform,
    entities->size,
    view_proj,
    nodes[0].wvp,
    size_of_node_pool,
    sizeof(Simple_renderer::Node));


  Transform::transforms_to_world_mats(entities->transform,
    entities->size,
    nodes[0].world_mat,
    size_of_node_pool,
    sizeof(Simple_renderer::Node));

  // Texture/vbo info
  for (std::size_t i = 0; i < entities->size; ++i)
  {
    nodes[i].vbo = model_pool->vbo[entities->model[i]];
    nodes[i].diffuse_id = Data::texture_pool_find(texture_pool, entities->texture[i])->texture_id;
  }

  Simple_renderer::render_nodes_fullbright(nodes, size_of_node_pool);
  //Simple_renderer::render_nodes_directional_light(renderer_nodes.data(), renderer_nodes.size(), &eye_pos[0]);

  math::mat4 wvp = math::mat4_multiply(math::mat4_id(), view, proj);
  Debug_line_renderer::render(math::mat4_get_data(wvp));
}


} // ns