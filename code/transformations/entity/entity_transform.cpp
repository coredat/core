#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_common.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <core/transform/transform.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/renderer_mesh_data.hpp>
#include <data/world_data/renderer_text_draw_calls_data.hpp>
#include <data/world_data/transform.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Entity_detail {


namespace {

inline void
update_transform(const util::generic_id this_id,
                 Data::Transform_data *transform_data,
                 const math::transform *transform)
{
  Data::data_lock(transform_data);

  size_t index;
  if(Data::transform_exists(transform_data, this_id, &index))
  {
    transform_data->field_transform[index] = *transform;
  }
  
  Data::data_unlock(transform_data);
}


inline void
update_mesh_renderer(const util::generic_id this_id,
                     World_data::Renderer_mesh_data *mesh_data,
                     const math::transform *transform)
{
  // Update mesh renderer data
  {
    World_data::data_lock(mesh_data);
  
    size_t mesh_index;

    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &mesh_index))
    {
      const math::mat4 world_mat = math::transform_get_world_matrix(*transform);
      memcpy(mesh_data->property_draw_call[mesh_index].world_matrix, &world_mat, sizeof(world_mat));
    }
    
    World_data::data_unlock(mesh_data);
  }
}


inline void
udpate_text_renderer(const util::generic_id this_id,
                     World_data::Renderer_text_draw_calls_data *text_data,
                     const math::transform *transform)
{
  assert(text_data);

  // Update mesh renderer data
  {
    World_data::data_lock(text_data);
  
    size_t index;

    if(World_data::renderer_text_draw_calls_data_exists(text_data, this_id, &index))
    {
      const math::mat4 world_mat = math::transform_get_world_matrix(*transform);
      memcpy(text_data->property_draw_call[index].world_matrix, &world_mat, sizeof(world_mat));
    }
    
    World_data::data_unlock(text_data);
  }
}


} // anon ns


void
set_transform(const util::generic_id this_id,
              World_data::Entity_data *entity_data,
              Data::Transform_data *transform_data,
              Data::Rigidbody_data *rb_data,
              World_data::Renderer_mesh_data *mesh_data,
              World_data::Renderer_text_draw_calls_data *text_data,
              const Core::Transform &set_transform,
              bool inform_phys_engine)
{
  assert(entity_data);
  assert(transform_data);
  assert(rb_data);
  assert(mesh_data);
  assert(text_data);

  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  // Get aabb
  math::aabb curr_aabb;
  {
    assert(transform_data);
  
    Data::data_lock(transform_data);
    Data::transform_get_aabb(transform_data, this_id, &curr_aabb);
    Data::data_unlock(transform_data);
  }
  
  // Update all the things that want to know.
  {
    // Build new transform
    const math::transform new_transform = math::transform_init(set_transform.get_position(),
                                                               set_transform.get_scale(),
                                                               set_transform.get_rotation());
    
    update_transform(this_id, transform_data, &new_transform);
    update_collider(this_id, entity_data, rb_data, &new_transform, &curr_aabb, inform_phys_engine);
    update_mesh_renderer(this_id, mesh_data, &new_transform);
    udpate_text_renderer(this_id, text_data, &new_transform);
  }
}


Core::Transform
get_core_transform(const util::generic_id this_id,
                   World_data::Entity_data *entity_data,
                   Data::Transform_data *transform_data)
{
  assert(entity_data);
  assert(transform_data);

  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return Core::Transform();
  }
  
  const math::transform transform_prop = get_transform(this_id, entity_data, transform_data);
  
  return Core::Transform(transform_prop.position, transform_prop.scale, transform_prop.rotation);
}


math::transform
get_transform(const util::generic_id this_id,
              World_data::Entity_data *entity_data,
              Data::Transform_data *transform_data)
{
  assert(transform_data);

  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return math::transform();
  }
  
  // Get Data
  math::transform return_transform;
  {
    Data::data_lock(transform_data);

    if(!Data::transform_get_transform(transform_data,
                                            this_id,
                                            &return_transform))
    {
      assert(false);
      LOG_WARNING(Error_string::data_not_found());
    }
    
    Data::data_unlock(transform_data);
  }
  
  return return_transform;
}





} // ns