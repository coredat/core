#ifndef WORLD_DATA_INCLUDED_4FAE9F04_0E15_4C3B_816B_F5C1FEA9EADA
#define WORLD_DATA_INCLUDED_4FAE9F04_0E15_4C3B_816B_F5C1FEA9EADA


#include "data_fwd.hpp"
#include <systems/physics/physics_fwd.hpp>
#include <systems/entity/entity_id.hpp>
#include <stdint.h>


namespace Data {


struct World
{
  Entity_pool             *entity_pool;
  Logic_pool              *logic_pool;
  Rigidbody_pool          *rigidbody_pool;
  Rigidbody_update_pool   *rigidbody_update_pool;
  Texture_pool            *texture_pool;
  Model_pool              *model_pool;
  Physics::World          *physics_world;
}; // struct


Entity
world_create_new_entity(World *world_data, const uint32_t type_id);


void
world_find_entity(Entity *out_entity, World *world_data, const ::Entity::Entity_id id);



} // ns


#endif // inc guard