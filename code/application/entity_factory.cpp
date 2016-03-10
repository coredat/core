#include "entity_factory.hpp"
#include <application/common/ids_object_tags.hpp>
#include <application/game_logic/actor_controller.hpp>
#include <application/game_logic/camera_controller.hpp>
#include <application/game_logic/bullet_controller.hpp>
#include <application/game_logic/gun_model.hpp>
#include <application/game_logic/move_model.hpp>
#include <core/interface/entity.hpp>
#include <core/physics/box_collider.hpp>
#include <application/game_logic/game_controller.hpp>
#include <application/game_logic/enemy_controller.hpp>


namespace Entity_factory {


Core::Entity
create_bullet(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, 1);
  
  entity.add_tag(Tag::projectile);
  
  // Logic
  entity.add_component<Bullet_controller>();
  
  const Core::Transform transform(math::vec3_zero(),
                                  math::vec3_init(0.25, 0.25, 0.25),
                                  math::quat_init());
  entity.set_transform(transform);
  
  const Core::Box_collider collider(transform.get_scale());
  //entity.set_collider(collider);
  
  entity.set_model_id(0);
  entity.set_material_id(1);
  
  return entity;
}


Core::Entity
create_game_play_camera(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, 1);
  
  entity.add_component<Camera_controller>();
  
  return entity;
}


Core::Entity
create_actor(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, 1);
  
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
  entity.set_material_id(2);
  
  return entity;
}


Core::Entity
create_game_state(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, 1);

  // Logic
  entity.add_component<Game_controller>();

  return entity;
}


Core::Entity
create_enemy(World_data::World *world)
{
  Core::Entity entity;
  World_data::world_create_new_entity(world, &entity, 1);
  
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
  entity.set_material_id(0);

  return entity;
}


} // ns