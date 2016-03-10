#include "move_model.hpp"
#include <core/interface/entity.hpp>
#include <core/transform/transform.hpp>
#include <assert.h>
#include "../level_functions.hpp"


void
Move_model::on_start()
{
  m_strafe = static_cast<float>(rand() & 100);
}


void
Move_model::on_update(const float dt)
{
  // Strafe
  if(m_pending_strafe != 0.f)
  {
    m_strafe += ((m_pending_strafe * m_move_speed) * dt);
    m_pending_strafe = 0.f;
    
    const math::vec2 level_position = Level::get_point_on_cirlce(m_strafe);
    
    Core::Transform trans = get_entity().get_transform();
    const math::vec3 position = trans.get_position();
    
    trans.set_position(math::vec3_init(
                        math::vec2_get_x(level_position),
                        math::vec2_get_y(level_position),
                        math::vec3_get_z(position)
                       ));
    
    get_entity().set_transform(trans);
  }
  
  switch(m_move_state)
  {
    case(Movement_state::jumped):
    {
      m_move_state = Movement_state::jumping;
      m_jump_time  = 0;
      
      break;
    }
    
    case(Movement_state::jumping):
    {
      m_jump_time += dt;
      float offset = (m_jump_speed * m_jump_time) + (m_gravity * m_jump_time * m_jump_time);
     
      Core::Transform trans = get_entity().get_transform();
      const math::vec3 curr_pos = trans.get_position();
      
      const float new_z = math::max(math::vec3_get_z(curr_pos) + offset, m_ground);
      
      if(new_z <= m_ground)
      {
        m_move_state = Movement_state::grounded;
      }
      
      const math::vec3 new_pos = math::vec3_init(
        math::vec3_get_x(curr_pos),
        math::vec3_get_y(curr_pos),
        new_z
      );
      trans.set_position(new_pos);
      
      get_entity().set_transform(trans);
      
      break;
    }
    
    default:
      break;
  };
}


void
Move_model::strafe_left(const float left)
{
  m_pending_strafe = left;
}


void
Move_model::jump()
{
  if(m_move_state == Movement_state::grounded)
  {
    m_move_state = Movement_state::jumped;
  }
}


void
Move_model::duck()
{
}