#include "move_model.hpp"
#include <core/interface/entity.hpp>
#include <core/transform/transform.hpp>
#include <assert.h>
#include "../level_functions.hpp"
#include <iostream>


void
Move_model::on_start()
{
}


void
Move_model::on_update(const float dt)
{
  // Set transform
  {
    const math::vec2 level_position = Level::get_point_on_cirlce(m_strafe);
    
    Core::Transform trans = get_entity().get_transform();
    
    trans.set_position(math::vec3_init(
                       math::vec2_get_x(level_position),
                       math::vec2_get_y(level_position),
                       m_depth
                       ));
    
    get_entity().set_transform(trans);
  }
  
  // States
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
      
      m_depth = math::max(m_depth + offset, m_ground);
      
      if(m_depth <= m_ground)
      {
        m_move_state = Movement_state::grounded;
      }
      
      const math::vec3 new_pos = math::vec3_init(
        math::vec3_get_x(curr_pos),
        math::vec3_get_y(curr_pos),
        m_depth
      );
      trans.set_position(new_pos);
      
      get_entity().set_transform(trans);
      
      break;
    }
    
    default:
      break;
  };
  
  // Check if entity is out the map
  if(m_depth < Level::get_far_death_zone() ||
     m_depth > Level::get_near_death_zones())
  {
    get_entity().destroy();
  }
}


void
Move_model::strafe_left(const float left)
{
  m_strafe += left;
}


void
Move_model::climb_up(const float up)
{
  if(m_move_state == Movement_state::grounded)
  {
    m_depth += up;
  }
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