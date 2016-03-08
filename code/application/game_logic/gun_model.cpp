#include "gun_model.hpp"
#include "../entity_factory.hpp"
#include "bullet_controller.hpp"
#include <core/interface/entity.hpp>


void
Gun_model::on_update(const float dt)
{
  switch(m_curr_state)
  {
    case(Gun_state::fired):
    {
      Core::Entity bullet = Entity_factory::create_bullet(m_world_data);

      Bullet_controller *bullet_controller = bullet.get_component<Bullet_controller>(Component_type::bullet_controller);
      assert(bullet_controller);
      
      if(bullet_controller)
      {
        bullet_controller->set_position(get_entity().get_transform().get_position());
        bullet_controller->set_direction(-1);
        
        m_curr_state = Gun_state::loading;
        m_curr_cooldown = 0.f;
      }

      break;
    }
    
    case(Gun_state::loading):
    {
      m_curr_cooldown += dt;
      
      if(m_curr_cooldown > m_max_cooldown)
      {
        m_curr_state = Gun_state::ready;
      }
      
      break;
    }
    
    default:
      break;
  };
}



void
Gun_model::fire_gun()
{
  if(m_curr_state == Gun_state::ready)
  {
    m_curr_state = Gun_state::fired;
  }
}