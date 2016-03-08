#include "camera_controller.hpp"
#include "../common/ids_object_tags.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
#include <iostream>


void
Camera_controller::on_start()
{
  m_camera.set_attached_entity(get_entity());
  m_camera.set_peer_priority(0, 1);
  
  // Set transform to above the player zone.
  
  Core::Transform curr_transform = get_entity().get_transform();
  
  math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
  
  curr_transform.set_position(math::vec3_init(0,0,5));
//  curr_transform.set_rotation(rot);
  
  get_entity().set_transform(curr_transform);
}



void
Camera_controller::on_end()
{
  m_camera.set_attached_entity(Core::Entity());
}


void
Camera_controller::on_update(const float dt)
{
  // Need to search for active players.
  uint32_t number_of_found_players(0);
  std::array<Core::Entity_id, 4> found_player_id;
  World_data::world_find_entities_with_tag(m_world_data,
                                           Tag::player,
                                           &number_of_found_players,
                                           &found_player_id[0],
                                           found_player_id.size());
  
  if(number_of_found_players)
  {
    Core::Entity entity;
    World_data::world_find_entity(m_world_data, &entity, found_player_id[0]);
    
    const Core::Transform entity_trans = entity.get_transform();
    Core::Transform this_trans = get_entity().get_transform();
    
    this_trans.set_position(
      math::vec3_init(
        math::vec3_get_x(this_trans.get_position()),
        math::vec3_get_y(this_trans.get_position()),
        math::vec3_get_z(this_trans.get_position())
        //math::vec3_get_z(entity_trans.get_position()) + 7.f
      )
    );
    
    get_entity().set_transform(this_trans);
  }
}

