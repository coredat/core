#include "application_host.hpp"
#include "entity_factory.hpp"
#include "game_logic/actor_local_player.hpp" // KILL!
#include "game_logic/actor_network_player.hpp" // KILL!
#include <data/actor/actor.hpp> // KILL!!!
#include <data/entity_pool.hpp>
#include <data/logic_pool.hpp>
#include <data/pending_rigidbody_pool.hpp>
#include <data/texture_pool.hpp>
#include <data/model_pool.hpp>
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
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
  Data::Pending_rigidbody_pool *pending_rbs,
  Data::Model_pool *model_pool,
  Data::Texture_pool *texture_pool,
  Physics::World *phy_world,
  Network::Connection *connection)
{
  
//  Entity::Entity_id kine_actor_network;
//  Entity::Entity_id kine_actor_local;

  {
    kine_actor_local = Entity_factory::create_kinematic_actor(entity_pool, pending_rbs, model_pool, texture_pool);
    kine_actor_network = Entity_factory::create_kinematic_actor(entity_pool, pending_rbs, model_pool, texture_pool);
  }
  
  Entity_factory::create_ground(entity_pool, pending_rbs, model_pool, texture_pool);


  // Game Logic
  {
    const auto free_slot = Data::logic_pool_get_slot(logic_pool);
    new(free_slot) Actor_local_player();
    
    auto base = reinterpret_cast<Logic::Base*>(logic_pool->objects_in_use[0]);
    base->set_entity(kine_actor_local);
    base->set_entity_data(entity_pool);
    base->set_physics_data(phy_world);
    base->model_pool = model_pool;
    base->texture_pool = texture_pool;
    base->pending_rbs = pending_rbs;
  }
  
  {
    const auto free_slot = Data::logic_pool_get_slot(logic_pool);
    new(free_slot) Actor_network_player();
    
    auto base = reinterpret_cast<Logic::Base*>(logic_pool->objects_in_use[1]);
    base->set_entity(kine_actor_network);
    base->set_entity_data(entity_pool);
    base->set_physics_data(phy_world);
    base->model_pool = model_pool;
    base->texture_pool = texture_pool;
    base->pending_rbs = pending_rbs;
  }
  
  for(auto &obj : logic_pool->objects_in_use)
  {
    reinterpret_cast<Logic::Base*>(obj)->on_start(); // TODO: reinter_cast?
  }
  
  Entity_factory::create_connection_node(entity_pool, pending_rbs, model_pool, texture_pool);
  
  const uint32_t number_of_random_cubes = 0;
  for(uint32_t i = 0; i < number_of_random_cubes; ++i)
  {
    Entity_factory::create_random_cube(entity_pool, pending_rbs, model_pool, texture_pool);
  }
}


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
  Physics::world_step(phy_world, delta_time);

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
  
  actor->look_up(static_cast<float>(inputs->controllers[0].axis_1[1]) * delta_time);
  actor->turn_right(static_cast<float>(inputs->controllers[0].axis_1[0]) * delta_time);

  if (inputs->controllers[0].buttons[Environment::Button::action_button] == (uint8_t)Environment::Button_action::on_down)
  {
    actor->action();
  }

  // ** Game Logic Update ** //

  for (auto &obj : logic_pool->objects_in_use)
  {
    reinterpret_cast<Logic::Base*>(obj)->on_update(delta_time); // TODO: reinter_cast?
  }


  // ** World Update and Render ** //

  if (pending_rbs->size)
  {
    Physics::colliders_generate(pending_rbs->rigidbody_collider,
                                pending_rbs->size,
                                pending_rbs->rigidbody_out,
                                pending_rbs->size);
    
    Physics::world_add_rigidbodies(phy_world,
      pending_rbs->rigidbody_property,
      pending_rbs->size,
      pending_rbs->rigidbody_out,
      pending_rbs->size);

    Data::pending_rigidbody_pool_clear(pending_rbs);
  }

  Network::send_packet(connection, sizeof(entity_pool->transform), entity_pool->transform, false);

}


} // ns