#include "world_data.hpp"
#include "entity_pool.hpp"
#include "pending_rigidbody_pool.hpp"
#include "entity.hpp"

#include <systems/physics/physics.hpp>

#include <atomic>


namespace
{
  std::atomic<uint32_t> instance(0);
}


namespace Data {


void
world_push_new_entity(World *world_data, const Entity *entity)
{
  Entity_pool *entity_pool = world_data->entity_pool;

  size_t empty_index;
  ::Entity::find_index_linearly(&empty_index, ::Entity::invalid_id(), entity_pool->entity_id, entity_pool->size);

  // Push entity data
  const ::Entity::Entity_id id = ::Entity::Entity_id{entity->get_id().type_id, ++instance};
  entity_pool->entity_id[empty_index] = id;
  entity_pool->transform[empty_index] = entity->get_transform();
  entity_pool->model[empty_index] = (Resource::Model::ENUM)entity->get_model_id();
  entity_pool->texture[empty_index] = (Resource::Texture::ENUM)entity->get_material_id();
  entity_pool->rigidbody_collider[empty_index] = entity->get_rigidbody_collider();
  entity_pool->rigidbody_property[empty_index] = entity->get_rigidbody_properties();
  
  entity_pool->rigidbody[empty_index].motion_state.reset(new Physics::Motion_state(entity_pool->entity_id[empty_index], entity_pool));

  if(entity->get_rigidbody_collider().collider_type != Physics::Collider_type::none)
  {
    // Generate collider.
    auto rb =  &entity_pool->rigidbody[empty_index];
    Physics::colliders_generate(&entity_pool->rigidbody_collider[empty_index], 1, &rb, 1);
    
    Pending_rigidbody_pool *pending_rb_pool = world_data->pending_rbs;
    
    // Push into pending rbs.
    Data::pending_rigidbody_pool_push(pending_rb_pool, entity_pool->rigidbody_property[empty_index], entity_pool->rigidbody_collider[empty_index], &entity_pool->rigidbody[empty_index]);
  }
  
}


} // ns