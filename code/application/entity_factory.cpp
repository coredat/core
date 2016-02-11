#include "entity_factory.hpp"
#include "resources.hpp"
#include "common/ids_object_types.hpp"
#include "common/ids_object_tags.hpp"
#include <systems/physics/physics.hpp>
#include <core/entity_id.hpp>
#include <systems/entity/generic_id.hpp>
#include <data/data.hpp>
#include <core/interface/entity.hpp>
#include <application/game_logic/actor_model.hpp>
#include <application/game_logic/local_player_controller.hpp>
#include <application/game_logic/npc_actor_controller.hpp>
#include <application/game_logic/network_player_controller.hpp>


namespace Entity_factory {


Core::Entity
create_ground(World_data::World *data)
{
  Core::Entity entity;
  assert(World_data::world_create_new_entity(data, &entity, Object_type::dev_static_ground));

  const float scale = 100.f;
  
  entity.set_transform(math::transform_init(math::vec3_zero(), math::vec3_init(scale,scale,scale), math::quat_init()));

  Physics::Rigidbody_properties rb_props;
  rb_props.mass = 0.f;
  entity.set_rigidbody_properties(rb_props);

  Physics::Rigidbody_collider rb_collider;
  rb_collider.collider_type = Physics::Collider_type::static_plane;
  rb_collider.collider_info.static_plane.normal[0] = 0;
  rb_collider.collider_info.static_plane.normal[1] = 1;
  rb_collider.collider_info.static_plane.normal[2] = 0;
  rb_collider.collider_info.static_plane.offset    = 0;
  entity.set_rigidbody_collider(rb_collider);

  entity.set_material_id(Resource::Texture::dev_squares);
  entity.set_model_id(Resource::Model::unit_plane);
  
  return entity;
}


Core::Entity
create_random_cube(World_data::World *world)
{
  Core::Entity entity;
  assert(World_data::world_create_new_entity(world, &entity, Object_type::dev_dynamic_cube));
  
  const float scale_x = static_cast<float>(rand() % 200 + 10) / 200;
  const float scale_y = static_cast<float>(rand() % 200 + 10) / 200;
  const float scale_z = static_cast<float>(rand() % 200 + 10) / 200;

  const float pos_x = static_cast<float>(rand() % 200 + 10) / 200;
  const float pos_y = static_cast<float>(rand() % 200 + 10) / 200;
  const float pos_z = static_cast<float>(rand() % 200 + 10) / 200;

  entity.set_transform(math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                       math::vec3_init(scale_x, scale_y, scale_z),
                       math::quat_init()));
  
  Physics::Rigidbody_properties rb_props;
  rb_props.mass = 1.f;
  entity.set_rigidbody_properties(rb_props);

  Physics::Rigidbody_collider rb_collider;
  rb_collider.collider_type = Physics::Collider_type::cube;
  rb_collider.collider_info.cube.extents[0] = scale_x * 0.5f;
  rb_collider.collider_info.cube.extents[1] = scale_y * 0.5f;
  rb_collider.collider_info.cube.extents[2] = scale_z * 0.5f;
  entity.set_rigidbody_collider(rb_collider);

  entity.set_material_id(static_cast<Resource::Texture::ENUM>(rand() % Resource::Texture::size));
  entity.set_model_id(Resource::Model::unit_cube);
  
  return entity;
}


Core::Entity
create_local_kinematic_actor(World_data::World *world)
{
  Core::Entity entity;
  assert(World_data::world_create_new_entity(world, &entity, Object_type::dev_actor));
  
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
  
  entity.add_component<Actor_model>();
  entity.add_component<Local_player_controller>();
  
  // Child
  Core::Entity child_entity;
  assert(World_data::world_create_new_entity(world, &child_entity, Object_type::dev_actor));
  
  const float c_scale_x = 0.4f;
  const float c_scale_y = 1.5f;
  const float c_scale_z = 0.4f;

  const float c_pos_x = pos_x + 0.f;
  const float c_pos_y = pos_y - 1.1f;
  const float c_pos_z = pos_z + 0.f;

  child_entity.set_transform(math::transform_init(math::vec3_init(c_pos_x, c_pos_y, c_pos_z),
                                            math::vec3_init(c_scale_x, c_scale_y, c_scale_z),
                                            math::quat_init()));
  
  child_entity.set_material_id(Resource::Texture::dev_green);
  child_entity.set_model_id(Resource::Model::unit_cube);
  
  child_entity.set_parent(entity.get_id());
  
  return entity;
}


Core::Entity
create_network_kinematic_actor(World_data::World *world)
{
  Core::Entity entity;
  assert(World_data::world_create_new_entity(world, &entity, Object_type::dev_actor));
  
  const float scale_x = 0.5f;
  const float scale_y = 0.5f;
  const float scale_z = 0.5f;

  const float pos_x = 3.f;
  const float pos_y = 2.5f;
  const float pos_z = 0.f;

  entity.set_transform(math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                            math::vec3_init(scale_x, scale_y, scale_z),
                                            math::quat_init()));
  
  entity.set_material_id(Resource::Texture::dev_red);
  entity.set_model_id(Resource::Model::unit_cube);
  
  entity.add_component<Actor_model>();
  entity.add_component<Network_player_controller>();
  
  entity.add_tag(Tag::network_player);
  
  return entity;
}


Core::Entity
create_npc_actor(World_data::World *world)
{
  Core::Entity entity;
  assert(World_data::world_create_new_entity(world, &entity, Object_type::dev_actor));
  
  const float scale_x = 0.5f;
  const float scale_y = 0.5f;
  const float scale_z = 0.5f;

  const float pos_x = -5 + (rand() % 10);
  const float pos_y = 2.5f;
  const float pos_z = -30 + (rand() % 10);

  entity.set_transform(math::transform_init(math::vec3_init(pos_x, pos_y, pos_z),
                                            math::vec3_init(scale_x, scale_y, scale_z),
                                            math::quat_init()));
  
  entity.set_material_id(Resource::Texture::dev_orange);
  entity.set_model_id(Resource::Model::unit_cube);
  
  entity.add_component<Actor_model>();
  entity.add_component<Npc_actor_controller>();
  
  // Child
  Core::Entity child_entity;
  assert(World_data::world_create_new_entity(world, &child_entity, Object_type::dev_actor));
  
  const float c_scale_x = 0.4f;
  const float c_scale_y = 1.5f;
  const float c_scale_z = 0.4f;

  const float c_pos_x = pos_x + 0.f;
  const float c_pos_y = pos_y - 1.1f;
  const float c_pos_z = pos_z + 0.f;

  child_entity.set_transform(math::transform_init(math::vec3_init(c_pos_x, c_pos_y, c_pos_z),
                                            math::vec3_init(c_scale_x, c_scale_y, c_scale_z),
                                            math::quat_init()));
  
  child_entity.set_material_id(Resource::Texture::dev_red);
  child_entity.set_model_id(Resource::Model::unit_cube);
  
  child_entity.set_parent(entity.get_id());
  
  return entity;
}


Core::Entity
create_placement_cube(World_data::World *world)
{
  Core::Entity entity;
  assert(World_data::world_create_new_entity(world, &entity, Object_type::dev_view_cube));

  entity.set_transform(math::transform_init(math::vec3_zero(),
                                            math::vec3_init(0.1f, 0.1f, 0.1f),
                                            math::quat_init()));

  Physics::Rigidbody_properties rb_props;
  rb_props.mass = 0;

  entity.set_rigidbody_properties(rb_props);
  entity.set_material_id(Resource::Texture::dev_blue);
  entity.set_model_id(Resource::Model::unit_cube);
  
  return entity;
}


Core::Entity
create_static_cube(World_data::World *world)
{
  Core::Entity entity;
  assert(World_data::world_create_new_entity(world, &entity, Object_type::dev_dynamic_cube));

  const float scale = 3.f + static_cast<float>(rand() % 10) / 2.f;
  const float scale_x = 1.f * scale;
  const float scale_y = math::g_ratio() * scale;
  const float scale_z = 1.f * scale;

  entity.set_transform(math::transform_init(math::vec3_zero(),
                                            math::vec3_init(scale_x, scale_y, scale_z),
                                            math::quat_init()));
  
  entity.set_rigidbody_properties(Physics::Rigidbody_properties{});

  Physics::Rigidbody_collider rb_collider;
  rb_collider.collider_type = Physics::Collider_type::cube;
  rb_collider.collider_info.cube.extents[0] = scale_x * 0.5f;
  rb_collider.collider_info.cube.extents[1] = scale_y * 0.5f;
  rb_collider.collider_info.cube.extents[2] = scale_z * 0.5f;
  entity.set_rigidbody_collider(rb_collider);

  entity.set_material_id(static_cast<Resource::Texture::ENUM>(rand() % Resource::Texture::size));
  entity.set_model_id(Resource::Model::unit_cube);
  
  return entity;
}


} // ns