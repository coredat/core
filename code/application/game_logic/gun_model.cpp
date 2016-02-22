#include "gun_model.hpp"
#include <math/math.hpp>
#include <iostream>


void
Gun_model::on_update(const float dt)
{
  switch(m_gun_state)
  {
    // The gun was fired.
    // We need to remove a bullet from the clip.
    // Then next state.
    case(State::fired):
    {
      if(!m_loaded_bullets)
      {
        m_gun_state = State::reloading;
        break;
      }
    
      --m_loaded_bullets;
      
      m_gun_state = State::firing;
      std::cout << "fired" << std::endl;
      break;
    }
  
    // The gun is fireing for as long as the timer is
    // running. Check the timer than next state.
    case(State::firing):
    {
      const util::milliseconds split = m_timer.currentResult();
      
      std::cout << split << std::endl;
      
      if(split > m_fire_time)
      {
        if(m_bullets)
        {
          std::cout << "ready" << std::endl;
          m_gun_state = State::ready;
        }
        else
        {
          std::cout << "empty" << std::endl;
          m_gun_state = State::empty;
        }
        
        m_timer.stop();
      }
      
      break;
    }
  
    // The gun is reloading.
    // After the timer is done we need to rmeove
    // the bullets from the bullet pool.
    // Then we make the gun 'ready'
    case(State::reloading):
    {
      const util::milliseconds split = m_timer.currentResult();
      
      if(split > m_reload_time)
      {
        // Bullets from pool.
        const uint32_t bullets = math::min(m_max_clip_size, m_bullets);
        
        std::cout << "reloaded" << std::endl;
        
        m_bullets -= bullets;
        m_loaded_bullets = bullets;
        
        m_gun_state = State::ready;
        m_timer.stop();
      }
      
      break;
    }
    
    // Nothing needs doing here.
    case(State::ready):
    case(State::empty):
    default:
      break;
  }; // state swtich
}


bool
Gun_model::fire_gun()
{
  if(m_gun_state == State::ready)
  {
    m_gun_state = State::fired;
    m_timer.start();
    
    return true;
  }
  
  return false;
}


bool
Gun_model::reload()
{
  if((m_gun_state == State::ready && m_loaded_bullets < m_max_clip_size)
  || (m_gun_state == State::empty && m_bullets))
  {
    m_gun_state = State::reloading;
    m_timer.start();
    
    return true;
  }
  
  return false;
}