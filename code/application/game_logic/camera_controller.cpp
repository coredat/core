#include "camera_controller.hpp"
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
  
}

