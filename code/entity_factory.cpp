#include "entity_factory.hpp"
#include <data/entity/entity_data.hpp>
#include <data/physics_world/collider_data.hpp>


namespace
{
  uint32_t instance = 0;
}


namespace Entity_factory {


void
create_ground(Entity::Data *data)
{
  const std::size_t index = data->add_entity(Entity_id{1, ++instance});
  
  data->get_collider_data()[index].type = Physics::Collider_type::static_plane;
  data->get_collider_data()[index].info.static_plane.normal[0] = 0;
  data->get_collider_data()[index].info.static_plane.normal[1] = 1;
  data->get_collider_data()[index].info.static_plane.normal[2] = 0;
  data->get_collider_data()[index].info.static_plane.offset = 0;
  data->get_collider_data()[index].mass = 0;
  
  data->get_transform_data()[index] = math::transform_init(math::vec3_zero(), math::vec3_init(10,10,10), math::quat_init());
  
  data->get_rigidbody_data()[index].motion_state.reset(new Data_detail::Motion_state(data->get_entity_data()[index], data));
}


void
create_random_cube(Entity::Data *data)
{
  const std::size_t index = data->add_entity(Entity_id{2, ++instance});
  
  data->get_rigidbody_data()[index].motion_state.reset(new Data_detail::Motion_state(data->get_entity_data()[index], data));
}


} // ns