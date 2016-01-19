#include "world_data.hpp"
#include "entity_pool.hpp"
#include "pending_rigidbody_pool.hpp"
#include "entity.hpp"

#include <systems/physics/physics.hpp>


namespace Data {


void
world_push_new_entity(World *world_data, const Entity *entity)
{
  Entity_pool *entity_pool = world_data->entity_pool;

  // Push entity data
  const ::Entity::Entity_id id = entity->get_id();
  
  size_t index;
  ::Entity::find_index_linearly(&index,
                                id,
                                world_data->entity_pool->entity_id,
                                world_data->entity_pool->size);
  
  entity_pool->entity_id[index]           = id;
  entity_pool->transform[index]           = entity->get_transform();
  entity_pool->model[index]               = (Resource::Model::ENUM)entity->get_model_id();
  entity_pool->texture[index]             = (Resource::Texture::ENUM)entity->get_material_id();
  entity_pool->rigidbody_collider[index]  = entity->get_rigidbody_collider();
  entity_pool->rigidbody_property[index]  = entity->get_rigidbody_properties();
  
  entity_pool->rigidbody[index].motion_state.reset(new Physics::Motion_state(entity_pool->entity_id[index], entity_pool));

  if(entity->get_rigidbody_collider().collider_type != Physics::Collider_type::none)
  {
    // Generate collider.
    auto rb =  &entity_pool->rigidbody[index];
    Physics::colliders_generate(&entity_pool->rigidbody_collider[index], 1, &rb, 1);
    
    Pending_rigidbody_pool *pending_rb_pool = world_data->pending_rbs;
    
    // Push into pending rbs.
    Data::pending_rigidbody_pool_push(pending_rb_pool,
                                      entity_pool->rigidbody_property[index],
                                      entity_pool->rigidbody_collider[index],
                                      &entity_pool->rigidbody[index]);
  }
}


Entity
world_create_new_entity(World *world_data, const uint32_t type_id)
{
  Entity new_entity(type_id);
  
  // Find empty index.
  size_t empty_index;
  ::Entity::find_index_linearly(&empty_index,
                                ::Entity::invalid_id(),
                                world_data->entity_pool->entity_id,
                                world_data->entity_pool->size);
  
  world_data->entity_pool->entity_id[empty_index] = new_entity.get_id();
  
  return new_entity;
}


} // ns