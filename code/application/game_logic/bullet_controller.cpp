#include "bullet_controller.hpp"
#include <core/transform/transform.hpp>
#include <core/interface/entity.hpp>
#include <iostream>



void
Bullet_controller::set_position(const math::vec3 position)
{
  Core::Transform curr_trans = get_entity().get_transform();
  curr_trans.set_position(position);
  get_entity().set_transform(curr_trans);
}


void
Bullet_controller::set_direction(const uint32_t i)
{
  if(i > 0)
  {
    m_direction = -1.f;
  }
  else
  {
    m_direction = 1.f;
  }
}


void
Bullet_controller::on_update(const float dt)
{
  const Core::Transform curr_trans = get_entity().get_transform();
  
  const math::vec3 curr_pos = curr_trans.get_position();
  const math::vec3 move_vec = math::vec3_init(0, 0, m_direction * (dt * 40));
  const math::vec3 new_pos  = math::vec3_add(curr_pos, move_vec);
  
  const Core::Transform new_trans(new_pos,
                                  curr_trans.get_scale(),
                                  curr_trans.get_rotation());

  get_entity().set_transform(new_trans);
  
  // Time to delete the bullet.
  {
    if(math::vec3_get_z(new_pos) > m_death_zone_top ||
       math::vec3_get_z(new_pos) < m_death_zone_bottom)
    {
      get_entity().destroy();
    }
  }
}


void
Bullet_controller::on_collision(const Core::Entity &collided_with)
{
  std::cout << "boom" << std::endl;
}