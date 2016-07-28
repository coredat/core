#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <core/transform/transform.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/renderer_mesh_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Entity_detail {


namespace
{

inline void
update_transform(const util::generic_id this_id,
                 World_data::World *world,
                 const math::transform *transform)
{
  auto transform_data = world->transform;
  
  World_data::data_lock(transform_data);

  size_t index;
  if(World_data::transform_data_exists(transform_data, this_id, &index))
  {
    transform_data->property_transform[index] = *transform;
  }
  
  World_data::data_unlock(transform_data);
}


inline void
update_mesh_renderer(const util::generic_id this_id,
                     World_data::World *world,
                     const math::transform *transform)
{
  auto mesh_data = world->mesh_data;

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




}


void
set_transform(const util::generic_id this_id,
              World_data::World *world,
              const Core::Transform &set_transform,
              bool inform_phys_engine)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return;
  }
  
  math::aabb curr_aabb;
  World_data::data_lock(world->transform);
  World_data::transform_data_get_property_aabb(world->transform, this_id, &curr_aabb);
  World_data::data_unlock(world->transform);
  
  const math::transform new_transform = math::transform_init(set_transform.get_position(),
                                                             set_transform.get_scale(),
                                                             set_transform.get_rotation());
  
  // TODO: Some possible async ness here?
  update_transform(this_id, world, &new_transform);
  update_collider(this_id, world, &new_transform, &curr_aabb, inform_phys_engine);
  update_mesh_renderer(this_id, world, &new_transform);
}


Core::Transform
get_transform(const util::generic_id this_id,
              World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return Core::Transform();
  }
  
  auto data = world->transform;

  size_t index;
  if(World_data::transform_data_exists(data, this_id, &index))
  {
    math::transform local_transform = data->property_transform[index];
    
    const math::vec3 pos = local_transform.position;
    const math::vec3 scale = local_transform.scale;
    
    return Core::Transform(pos,
                           scale,
                           local_transform.rotation);
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
  }
  
  return Core::Transform();
}




} // ns