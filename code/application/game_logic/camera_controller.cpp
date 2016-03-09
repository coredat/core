#include "camera_controller.hpp"
#include "../common/ids_object_tags.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
#include <algorithm>


void
Camera_controller::on_start()
{
  m_camera.set_attached_entity(get_entity());
  m_camera.set_peer_priority(0, 1);
  
  Core::Transform curr_transform = get_entity().get_transform();
  curr_transform.set_position(math::vec3_init(0,0,0));
  
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
  
  // We set the camera origin as the first point.
  math::vec3 accum_target = math::vec3_init(0,0,10.f);
  Core::Transform this_trans = get_entity().get_transform();
  
  // Go through the players and accumulate the target point.
  const uint32_t num_of_players = std::min<uint32_t>(number_of_found_players, found_player_id.size());
  
  for(uint32_t i = 0; i < num_of_players; ++i)
  {
    Core::Entity entity;
    World_data::world_find_entity(m_world_data, &entity, found_player_id[i]);

    const math::vec3 player_pos = entity.get_transform().get_position();
    
    const math::vec3 direction = math::vec3_subtract(player_pos, accum_target);
    const math::vec3 scaled_dir = math::vec3_scale(direction, 0.5f);
    
    accum_target = math::vec3_add(accum_target, scaled_dir);
  }
  
  m_target_position = accum_target;
  
  const math::vec3 this_pos = this_trans.get_position();

  const math::vec3 move_dir   = math::vec3_subtract(m_target_position, this_pos);
  const math::vec3 scaled_dir = math::vec3_scale(move_dir, dt * 2);
  const math::vec3 new_pos    = math::vec3_add(this_pos, scaled_dir);

  this_trans.set_position(new_pos);
  
  get_entity().set_transform(this_trans);
}