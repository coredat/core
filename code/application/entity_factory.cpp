#include "entity_factory.hpp"
#include "level_functions.hpp"
#include <application/common/ids_object_tags.hpp>
#include <application/common/ids_object_types.hpp>
#include <application/game_logic/actor_controller.hpp>
#include <application/game_logic/camera_controller.hpp>
#include <application/game_logic/bullet_controller.hpp>
#include <application/game_logic/player_stats_model.hpp>
#include <application/game_logic/enemy_spawner_model.hpp>
#include <application/game_logic/gun_model.hpp>
#include <application/game_logic/move_model.hpp>
#include <application/game_logic/explosion_model.hpp>
#include <application/game_logic/game_controller.hpp>
#include <application/game_logic/enemy_controller.hpp>
#include <application/game_logic/multiplier_controller.hpp>
#include <application/resources.hpp>
#include <core/interface/entity.hpp>
#include <core/physics/box_collider.hpp>


namespace Entity_factory {


Core::Entity
create_level(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::level);
  
  entity.set_name("Level");
  
  const float level_length = math::abs(Level::get_top_of_level() - Level::get_bottom_of_level());
  constexpr float level_origin = (Level::get_top_of_level() - Level::get_bottom_of_level()) * 0.5f;
  
  const Core::Transform transform(math::vec3_init(0, 0, -level_origin),
                                  math::vec3_init(Level::get_radius() * 2.1f, Level::get_radius() * 2.1f, level_length),
                                  math::quat_init());
  entity.set_transform(transform);
  
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);e
  
  entity.set_model_id(Resource::Model::unit_tube);
  entity.set_material_id(Resource::Texture::dev_green);
  
  return entity;
}


Core::Entity
create_bullet(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::bullet);
  
  entity.set_name("Bullet");
  entity.add_tag(Tag::projectile);
  
  // Logic
  entity.add_component<Bullet_controller>();
  
  const Core::Transform transform(math::vec3_zero(),
                                  math::vec3_init(0.25f, 0.25f, 1.5f),
                                  math::quat_init());
  entity.set_transform(transform);
  
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  entity.set_model_id(0);
  entity.set_material_id(Resource::Texture::dev_red);
  
  return entity;
}


Core::Entity
create_game_play_camera(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::game_camera);
  
  entity.set_name("Camera");
  
  entity.add_component<Camera_controller>();
  
  return entity;
}


Core::Entity
create_actor(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::player);
  
  entity.set_name("Player");
  
  // Logic
  entity.add_component<Actor_controller>();
  entity.add_component<Gun_model>();
  entity.add_component<Move_model>();
  
  
  // Transform
  const Core::Transform transform(math::vec3_init(0, 3, 0),
                                  math::vec3_init(0.5f, 0.5f, 1.f),
                                  math::quat_init());
  entity.set_transform(transform);
  
  
  // Physics
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  // Other stuff.
  entity.set_model_id(0);
  entity.set_material_id(Resource::Texture::dev_blue);
  
  return entity;
}


Core::Entity
create_game_state(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::game_state);
  
  entity.set_name("Game State");

  // Logic
  entity.add_component<Game_controller>();
  entity.add_component<Enemy_spawner_model>();
  entity.add_component<Player_stats_model>();

  return entity;
}


Core::Entity
create_enemy(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::npc);
  
  entity.set_name("Enemy");
  entity.add_tag(Tag::npc);
  
  // Logic
  entity.add_component<Enemy_controller>();
  entity.add_component<Gun_model>();
  entity.add_component<Move_model>();
  
  // Transform
  const Core::Transform transform(math::vec3_init(0, 3, 0),
                                  math::vec3_init(0.5f, 0.5f, 1.f),
                                  math::quat_init());
  entity.set_transform(transform);
  
  
  // Physics
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  // Other stuff.
  entity.set_model_id(0);
  entity.set_material_id(Resource::Texture::dev_orange);

  return entity;
}


Core::Entity
create_explosion(World_data::World *world, const Core::Transform transform)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::explosion);
  
  entity.set_name("Explosions");
  
  // Transform
  entity.set_transform(transform);
  
  entity.add_component<Explosion_model>();
  
  // Physics
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  // Other stuff.
  entity.set_model_id(0);
  entity.set_material_id(Resource::Texture::dev_squares);
  
  return entity;
}


Core::Entity
create_multiplier(World_data::World *world, const Core::Transform transform)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, Object_type::multiplier);
  
  entity.set_name("Multiplier");
  entity.add_tag(Tag::pickup);
  
  // Transform
  entity.set_transform(transform);
  
  // Add logic.
  entity.add_component<Multiplier_controller>();
  entity.add_component<Move_model>();
  
  // Physics
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  // Other stuff.
  entity.set_model_id(0);
  entity.set_material_id(Resource::Texture::dev_colored_squares);
  
  return entity;
}


} // ns