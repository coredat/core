#include "entity_factory.hpp"
#include <data/physics/physics.hpp>
#include <data/entity/entity_data.hpp>


namespace
{
  uint32_t instance = 0;
}


namespace Entity_factory {


Entity_id
create_ground(Entity::Data *data)
{
  const Entity_id id = Entity_id{1, ++instance};

  const std::size_t index = data->add_entity(id);
  
  data->get_collider_data()[index].collider_type = Physics::Collider_type::static_plane;
  data->get_collider_data()[index].collider_info.static_plane.normal[0] = 0;
  data->get_collider_data()[index].collider_info.static_plane.normal[1] = 1;
  data->get_collider_data()[index].collider_info.static_plane.normal[2] = 0;
  data->get_collider_data()[index].collider_info.static_plane.offset = 0;
  data->get_collider_data()[index].mass = 0;
  
  data->get_transform_data()[index] = math::transform_init(math::vec3_zero(), math::vec3_init(10,10,10), math::quat_init());
  
  data->get_rigidbody_data()[index].motion_state.reset(new Physics::Motion_state(data->get_entity_data()[index], data));
  
  return id;
}


Entity_id
create_random_cube(Entity::Data *data)
{
  const Entity_id id = Entity_id{2, ++instance};

  const float scale_x = static_cast<float>(rand() % 200 + 10) / 200;
  const float scale_y = static_cast<float>(rand() % 200 + 10) / 200;
  const float scale_z = static_cast<float>(rand() % 200 + 10) / 200;

  const float pos_x = static_cast<float>(rand() % 200 + 10) / 200;
  const float pos_y = static_cast<float>(rand() % 200 + 10) / 200;
  const float pos_z = static_cast<float>(rand() % 200 + 10) / 200;

  const std::size_t index = data->add_entity(id);
  
  data->get_transform_data()[index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                           math::vec3_init(scale_x, scale_y, scale_z),
                                                           math::quat_init());
  
  data->get_collider_data()[index].collider_type = Physics::Collider_type::cube;
  data->get_collider_data()[index].collider_info.cube.extents[0] = scale_x * 0.5;
  data->get_collider_data()[index].collider_info.cube.extents[1] = scale_y * 0.5;
  data->get_collider_data()[index].collider_info.cube.extents[2] = scale_z * 0.5;
  data->get_collider_data()[index].mass = 1;
  
  data->get_rigidbody_data()[index].motion_state.reset(new Physics::Motion_state(data->get_entity_data()[index], data));
  
  return id;
}


Entity_id
create_actor(Entity::Data *data)
{
  const Entity_id id = Entity_id{3, ++instance};

  const float scale_x = 1.f;
  const float scale_y = 2.f;
  const float scale_z = 1.f;

  const float pos_x = 0.f;
  const float pos_y = 2.5f;
  const float pos_z = 0.f;

  const std::size_t index = data->add_entity(id);
  
  data->get_transform_data()[index] = math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                                           math::vec3_init(scale_x, scale_y, scale_z),
                                                           math::quat_init());
  
  data->get_collider_data()[index].collider_type = Physics::Collider_type::cube;
  data->get_collider_data()[index].collider_info.cube.extents[0] = scale_x * 0.5;
  data->get_collider_data()[index].collider_info.cube.extents[1] = scale_y * 0.5;
  data->get_collider_data()[index].collider_info.cube.extents[2] = scale_z * 0.5;
  data->get_collider_data()[index].rotation_axis = Physics::Axis::y;
  data->get_collider_data()[index].mass = 1;
  
  data->get_rigidbody_data()[index].motion_state.reset(new Physics::Motion_state(data->get_entity_data()[index], data));
  
  return id;
}


} // ns