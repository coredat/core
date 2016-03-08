#include "move_model.hpp"
#include <core/interface/entity.hpp>
#include <core/transform/transform.hpp>
#include <assert.h>
#include "../level_functions.hpp"


void
Move_model::on_update(const float dt)
{
  // Strafe
  if(m_pending_strafe != 0.f)
  {
    m_strafe += (m_pending_strafe * dt);
    m_pending_strafe = 0.f;
    
    const math::vec3 position = Level::get_point_on_cirlce(m_strafe);
   
    Core::Transform curr_trans = get_entity().get_transform();
    curr_trans.set_position(position);
    
    get_entity().set_transform(curr_trans);
  }
  
  const float mass   = 0.2f;
  const float g      = 4.1f;
  static float time  = 0;
  time              += dt;

  switch(m_move_state)
  {
    case(Movement_state::jumped):
    {
      m_jump_momentum = mass * (20.4f);
      m_move_state = Movement_state::jumping;
      time = 0;
      
      break;
    }
    
    case(Movement_state::jumping):
    {
      float offset = m_jump_momentum * time - g*time*time;
     
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
      
    }
    
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