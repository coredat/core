#include "application_host.hpp"
#include "game_logic/actor_local_player.hpp" // KILL!
#include "game_logic/actor_network_player.hpp" // KILL!
#include <data/actor/actor.hpp> // KILL!!!
#include <data/entity_pool.hpp>
#include <data/logic_pool.hpp>
#include <data/pending_rigidbody_pool.hpp>
#include <systems/network/network.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>


namespace Application {


void
host_think(
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
  Data::Pending_rigidbody_pool *pending_rbs,
  Physics::World *phy_world,
  Network::Connection *connection,
  const Environment::Input *inputs,
  const float delta_time)
{
  // SORT THIS!
  const Entity::Entity_id kine_actor_local{0,0};
  const Entity::Entity_id kine_actor_network{0,0};

  Network::poll_events(connection,
    0,
    [&](const Network::Event_id id, const void *data, const std::size_t size_of_data)
  {
    const Actor::Input_cmds *cmds = reinterpret_cast<const Actor::Input_cmds*>(data);
    Actor::input(*cmds, delta_time, kine_actor_network, entity_pool, entity_pool->size, phy_world);
  },
    &std::cout);

  std::size_t index;
  Entity::find_index_linearly(&index, kine_actor_local, entity_pool->entity_id, entity_pool->size);

  Actor_local_player *actor = reinterpret_cast<Actor_local_player*>(logic_pool->objects_in_use[0]);

  actor->move_fwd(inputs->controllers[0].axis_2[1]);
  actor->move_right(inputs->controllers[0].axis_2[0]);

  if (inputs->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_down)
  {
    actor->action();
  }

  Network::send_packet(connection, sizeof(entity_pool->transform), entity_pool->transform, false);

  // ** Game Logic Update ** //

  for (auto &obj : logic_pool->objects_in_use)
  {
    reinterpret_cast<Logic::Base*>(obj)->on_update(delta_time); // TODO: reinter_cast?
  }


  // ** World Update and Render ** //

  if (pending_rbs->size)
  {
    Physics::world_add_rigidbodies(phy_world,
      pending_rbs->rigidbody_property,
      pending_rbs->size,
      pending_rbs->rigidbody_out,
      pending_rbs->size);

    Data::pending_rigidbody_pool_clear(pending_rbs);
  }

  Physics::world_step(phy_world, delta_time);
}


} // ns