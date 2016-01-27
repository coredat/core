#include "entity_factory.hpp"
#include "resources.hpp"
#include "object_types.hpp"
#include <systems/physics/physics.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/entity/generic_id.hpp>
#include <data/data.hpp>


namespace Entity_factory {


Data::Entity
create_ground(Data::World *data)
{
  Data::Entity entity;
  assert(Data::world_create_new_entity(data, &entity, Object_type::dev_static_ground));

  const float scale = 10.f;
  
  entity.set_transform(math::transform_init(math::vec3_zero(), math::vec3_init(scale,scale,scale), math::quat_init()));
  entity.set_rigidbody_properties(Physics::Rigidbody_properties{0.f});
  entity.set_rigidbody_collider(Physics::Rigidbody_collider{Physics::Collider_type::static_plane, 0, 1, 0, 0});
  entity.set_material_id(Resource::Texture::dev_squares);
  entity.set_model_id(Resource::Model::unit_plane);
  
  return entity;
}


Data::Entity
create_random_cube(Data::World *world)
{
  Data::Entity entity;
  assert(Data::world_create_new_entity(world, &entity, Object_type::dev_dynamic_cube));
  
  const float scale_x = static_cast<float>(rand() % 200 + 10) / 200;
  const float scale_y = static_cast<float>(rand() % 200 + 10) / 200;
  const float scale_z = static_cast<float>(rand() % 200 + 10) / 200;

  const float pos_x = static_cast<float>(rand() % 200 + 10) / 200;
  const float pos_y = static_cast<float>(rand() % 200 + 10) / 200;
  const float pos_z = static_cast<float>(rand() % 200 + 10) / 200;

  entity.set_transform(math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                       math::vec3_init(scale_x, scale_y, scale_z),
                       math::quat_init()));
  
  entity.set_rigidbody_properties(Physics::Rigidbody_properties{1.f});
  entity.set_rigidbody_collider(Physics::Rigidbody_collider{Physics::Collider_type::cube, scale_x * 0.5f, scale_y * 0.5f, scale_z * 0.5f});
  entity.set_material_id(static_cast<Resource::Texture::ENUM>(rand() % Resource::Texture::size));
  entity.set_model_id(Resource::Model::unit_cube);
  
  return entity;
}


Data::Entity
create_kinematic_actor(Data::World *world)
{
  Data::Entity entity;
  assert(Data::world_create_new_entity(world, &entity, Object_type::dev_static_ground));
  
  const float scale_x = 0.5f;
  const float scale_y = 0.5f;
  const float scale_z = 0.5f;

  const float pos_x = 3.f;
  const float pos_y = 2.5f;
  const float pos_z = 0.f;

  entity.set_transform(math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                            math::vec3_init(scale_x, scale_y, scale_z),
                                            math::quat_init()));
  
  entity.set_material_id(Resource::Texture::dev_orange);
  entity.set_model_id(Resource::Model::unit_cube);
  
  return entity;
}


Data::Entity
create_placement_cube(Data::World *world)
{
  Data::Entity entity;
  assert(Data::world_create_new_entity(world, &entity, Object_type::dev_view_cube));

  entity.set_transform(math::transform_init(math::vec3_zero(),
                                            math::vec3_init(0.1f, 0.1f, 0.1f),
                                            math::quat_init()));

  entity.set_rigidbody_properties(Physics::Rigidbody_properties{0});
  entity.set_material_id(Resource::Texture::dev_blue);
  entity.set_model_id(Resource::Model::unit_cube);
  
  return entity;
}


Data::Entity
create_connection_node(Data::World *world)
{
  Data::Entity entity;
  assert(Data::world_create_new_entity(world, &entity, Object_type::dev_dynamic_cube));

  const float scale = 1.f;
  const float scale_x = 1.f * scale;
  const float scale_y = math::g_ratio() * scale;
  const float scale_z = 1.f * scale;

  entity.set_transform(math::transform_init(math::vec3_zero(),
                                            math::vec3_init(scale_x, scale_y, scale_z),
                                            math::quat_init()));
  
  entity.set_rigidbody_properties(Physics::Rigidbody_properties{1.f});
  entity.set_rigidbody_collider(Physics::Rigidbody_collider{Physics::Collider_type::cube, scale_x * 0.5f, scale_y * 0.5f, scale_z * 0.5f});
  entity.set_material_id(static_cast<Resource::Texture::ENUM>(rand() % Resource::Texture::size));
  entity.set_model_id(Resource::Model::unit_cube);
  
  return entity;
}


} // ns