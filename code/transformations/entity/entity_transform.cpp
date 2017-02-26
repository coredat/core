#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_common.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/transform/transform.hpp>
#include <data/world_data.hpp>
#include <data/graph/graph.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/graph/graph.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/renderers/text/text_renderer.hpp>
#include <common/error_strings.hpp>
#include <common/data_types.hpp>
#include <math/math.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Entity_detail {


namespace {


inline void
update_mesh_renderer(const uint32_t this_id,
                     Data::Mesh_draw_call_data *mesh_data,
                     const math::transform *transform)
{
  // Update mesh renderer data
  {
    Data::data_lock(mesh_data);
  
    size_t mesh_index;

    if(Data::mesh_draw_call_exists(mesh_data, this_id, &mesh_index))
    {
      const math::mat4 world_mat = math::transform_get_world_matrix(*transform);
      memcpy(
        mesh_data->field_draw_call[mesh_index].world_matrix,
        &world_mat,
        sizeof(world_mat)
      );
    }
    
    Data::data_unlock(mesh_data);
  }
  
  
} // anon ns


inline void
udpate_text_renderer(const uint32_t this_id,
                     Data::Text_draw_call_data *text_data,
                     const math::transform *transform)
{
  assert(text_data);

  // Update mesh renderer data
  {
    const math::mat4 world_mat = math::transform_get_world_matrix(*transform);
  
    Data::data_lock(text_data);
  
    size_t index;

    if(Data::text_draw_call_exists(text_data, this_id, &index))
    {
      memcpy(
        text_data->field_draw_call[index].world_matrix,
        &world_mat,
        sizeof(world_mat));
    }
    
    Data::data_unlock(text_data);
    
    // -- New Text Renderer -- //
    {
      auto world = Core_detail::world_index_get_world_data(1);
      Data::Text_renderer::update_draw_call_matrix(world->text_renderer, this_id, math::mat4_get_data(world_mat));
    }
  }
}


} // anon ns


void
set_transform(const uint32_t this_id,
              Data::Graph::Graph_data *entity_data,
              Data::Rigidbody_data *rb_data,
              Bullet_data::World *phy_world,
              Data::Trigger_data *trigger_data,
              Data::Mesh_draw_call_data *mesh_data,
              Data::Text_draw_call_data *text_data,
              const Core::Transform &set_transform,
              bool inform_phys_engine)
{
  LOG_TODO_ONCE("asserts need to be conditional depending on what transforms are betings set.")
//  assert(entity_data);
//  assert(transform_data);
//  assert(rb_data);
//  assert(mesh_data);
//  assert(text_data);

  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  auto world = Core_detail::world_index_get_world_data(1);
  
  uint32_t components(0);
  Data::Graph::components_get(world->scene_graph, this_id, &components);
  {
//    Data::data_lock(entity_data);
//    Data::entity_get_components(entity_data, this_id, &components);
//    Data::data_unlock(entity_data);
  }
  
  // Get aabb
  math::aabb curr_aabb;
  Data::Graph::aabb_get(world->scene_graph, this_id, &curr_aabb);
  {
//    assert(transform_data);
//  
//    Data::data_lock(transform_data);
//    Data::transform_get_aabb(transform_data, this_id, &curr_aabb);
//    Data::data_unlock(transform_data);
  }
  
  // Update all the things that want to knowx.
  {
    // Build new transform
    const math::transform new_transform(
      math::transform_init(
        set_transform.get_position(),
        set_transform.get_scale(),
        set_transform.get_rotation()
      )
    );

//    update_transform(this_id, transform_data, &new_transform);

    if(inform_phys_engine && Common::Data_type::is_collidable(components))
    {
      Entity_detail::set_phy_transform(this_id, &set_transform, entity_data, rb_data, phy_world, trigger_data);
    }

    update_mesh_renderer(this_id, mesh_data, &new_transform);
    
    if(components & Common::Data_type::renderer_text)
    {
      udpate_text_renderer(this_id, text_data, &new_transform);
    }
  }
}


Core::Transform
get_core_transform(const uint32_t this_id,
                   Data::Graph::Graph_data *entity_data,
                   const Data::Transform_data *transform_data)
{
  assert(entity_data);
  assert(transform_data);

  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return Core::Transform();
  }
  
  const math::transform transform_prop(
    get_transform(this_id, entity_data, transform_data)
  );
  
  return Core::Transform(
    transform_prop.position,
    transform_prop.scale,
    transform_prop.rotation
  );
}


math::transform
get_transform(const uint32_t this_id,
              Data::Graph::Graph_data *entity_data,
              const Data::Transform_data *transform_data)
{
  assert(transform_data);

  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return math::transform();
  }
  
  auto world = Core_detail::world_index_get_world_data(1);
  
  // Get Data
  math::transform return_transform;
  Data::Graph::transform_get(world->scene_graph, this_id, &return_transform);
  {
//    Data::data_lock(transform_data);
//
//    if(!Data::transform_get_transform(transform_data,
//                                            this_id,
//                                            &return_transform))
//    {
//      assert(false);
//      LOG_WARNING(Error_string::data_not_found());
//    }
//    
//    Data::data_unlock(transform_data);
  }
  
  return return_transform;
}





} // ns
