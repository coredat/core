#include "application_host.hpp"
#include "entity_factory.hpp"
#include "game_logic/actor_local_player.hpp" // KILL!
#include "game_logic/actor_network_player.hpp" // KILL!
#include <data/data.hpp>
#include <systems/network/network.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>


namespace
{
  Entity::Entity_id kine_actor_local{4,1};
  Entity::Entity_id kine_actor_network{4,2};
}


namespace Application {


void
host_initialize(
  Data::World *world,
  Network::Connection *connection)
{
  
//  Entity::Entity_id kine_actor_network;
//  Entity::Entity_id kine_actor_local;

  Entity_factory::create_ground(world);

  {
    kine_actor_local = Entity_factory::create_kinematic_actor(world).get_id();
    kine_actor_network = Entity_factory::create_kinematic_actor(world).get_id();
  }
  

  // Game Logic
  {
    const auto free_slot = Data::logic_pool_get_slot(world->logic_pool);
    new(free_slot) Actor_local_player();
    
    auto base = reinterpret_cast<Logic::Base*>(world->logic_pool->objects_in_use[0]);
    base->set_entity(kine_actor_local);
    base->world_data = world;
    base->m_world = world->physics_world;
  }
  
  {
    const auto free_slot = Data::logic_pool_get_slot(world->logic_pool);
    new(free_slot) Actor_network_player();
    
    auto base = reinterpret_cast<Logic::Base*>(world->logic_pool->objects_in_use[1]);
    base->set_entity(kine_actor_network);
    base->world_data = world;
    base->m_world = world->physics_world;
  }
  
  for(auto &obj : world->logic_pool->objects_in_use)
  {
    reinterpret_cast<Logic::Base*>(obj)->on_start(); // TODO: reinter_cast?
  }
  
  Entity_factory::create_connection_node(world);
  
  const uint32_t number_of_random_cubes = 0;
  for(uint32_t i = 0; i < number_of_random_cubes; ++i)
  {
    Entity_factory::create_random_cube(world);
  }
}


void
host_think(
  Data::World *world,
  Network::Connection *connection,
  const Environment::Input *inputs,
  const float delta_time)
{
  Physics::world_step(world->physics_world, delta_time);

  Network::poll_events(connection,
    0,
    [&](const Network::Event_id id, const void *data, const std::size_t size_of_data)
  {
//    const Actor::Input_cmds *cmds = reinterpret_cast<const Actor::Input_cmds*>(data);
//    Actor::input(*cmds, delta_time, kine_actor_network, world->entity_pool, world->entity_pool->size, world->physics_world);
  },
    &std::cout);

  std::size_t index;
  Entity::find_index_linearly(&index, kine_actor_local, world->entity_pool->entity_id, world->entity_pool->size);

  Actor_local_player *actor = reinterpret_cast<Actor_local_player*>(world->logic_pool->objects_in_use[0]);

  actor->move_fwd(inputs->controllers[0].axis_2[1]);
  actor->move_right(inputs->controllers[0].axis_2[0]);
  
  actor->look_up(static_cast<float>(inputs->controllers[0].axis_1[1]) * delta_time);
  actor->turn_right(static_cast<float>(inputs->controllers[0].axis_1[0]) * delta_time);

  if (inputs->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_down)
  {
    actor->action();
  }

  // ** Game Logic Update ** //

  for (auto &obj : world->logic_pool->objects_in_use)
  {
    reinterpret_cast<Logic::Base*>(obj)->on_update(delta_time); // TODO: reinter_cast?
  }

  // Push in new phy entities.
  Data::rigidbody_pool_process_updates(world->physics_world, world, world->rigidbody_update_pool, world->rigidbody_pool);
  Data::rigidbody_update_pool_clear(world->rigidbody_update_pool);

  Network::send_packet(connection, sizeof(world->entity_pool->transform), world->entity_pool->transform, false);

}


} // ns