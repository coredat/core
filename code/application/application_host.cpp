#include "application_host.hpp"
#include "entity_factory.hpp"
#include "game_logic/actor_local_player.hpp" // KILL!
#include "game_logic/actor_network_player.hpp" // KILL!
#include <data/data.hpp>
#include <systems/network/network.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>
#include <application_interface/entity.hpp>
#include <iostream>


namespace
{
  Core::Entity_id kine_actor_local{4,1};
  Core::Entity_id kine_actor_network{4,2};
}


namespace Application {


void
host_initialize(
  Data::World *world,
  Network::Connection *connection)
{
  Entity_factory::create_ground(world);

  {
    kine_actor_local   = Entity_factory::create_local_kinematic_actor(world).get_id();
    kine_actor_network = Entity_factory::create_network_kinematic_actor(world).get_id();
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
  Core::find_index_linearly(&index, kine_actor_local, world->entity_pool->entity_id, world->entity_pool->size);

  // Kill me!!!!
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
  Data::logic_pool_on_start_hook(world->logic_pool);
  Data::logic_pool_on_update_hook(world->logic_pool, delta_time);

  // Push in new phy entities.  
  Data::rigidbody_pool_update_scene_graph_changes(world->rigidbody_pool, world, world->entity_graph_changes);
  
  // Reset the entity pool for new changes.
  Data::entity_graph_change_pool_init(world->entity_graph_changes);

  Network::send_packet(connection, sizeof(world->entity_pool->transform), world->entity_pool->transform, false);

}


} // ns