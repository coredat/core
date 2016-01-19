#ifndef WORLD_DATA_INCLUDED_4FAE9F04_0E15_4C3B_816B_F5C1FEA9EADA
#define WORLD_DATA_INCLUDED_4FAE9F04_0E15_4C3B_816B_F5C1FEA9EADA


#include "data_fwd.hpp"


namespace Data {


struct World
{
  Entity_pool             *entity_pool;
  Logic_pool              *logic_pool;
  Pending_rigidbody_pool  *pending_rbs;
  Rigidbody_pool          *rigidbody_pool;
  Texture_pool            *texture_pool;
  Model_pool              *model_pool;
}; // struct


void
world_push_new_entity(World *world_data, const Entity *entity);


} // ns


#endif // inc guard