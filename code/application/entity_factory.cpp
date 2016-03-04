#include <core/interface/entity.hpp>
#include <core/physics/box_collider.hpp>
#include "entity_factory.hpp"


namespace Entity_factory {


Core::Entity
create_ground(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, 1);
  
  const Core::Transform transform(math::vec3_zero(), math::vec3_init(10, 0.5, 10), math::quat_init());
  entity.set_transform(transform);
  
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  entity.set_model_id(0);
  entity.set_material_id(1);
  
  return entity;
}



Core::Entity
create_actor(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, 1);
  
  const Core::Transform transform(math::vec3_zero(), math::vec3_init(1, 3, 1), math::quat_init());
  entity.set_transform(transform);
  
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  entity.set_model_id(0);
  entity.set_material_id(2);
  
  return entity;
}


} // ns