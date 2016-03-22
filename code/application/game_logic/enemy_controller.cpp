#include "enemy_controller.hpp"
#include "move_model.hpp"
#include <core/interface/entity.hpp>
#include "../common/ids_object_tags.hpp"
#include "../level_functions.hpp"
#include "../entity_factory.hpp"


void
Enemy_controller::on_start()
{
  Core::Transform trans = get_entity().get_transform();
  trans.set_position(math::vec3_init(0, 0, Level::get_bottom_of_level()));
  get_entity().set_transform(trans);
  
  Move_model *move = get_entity().get_component<Move_model>(Component_type::move_model);
  assert(move);
  
  if(move)
  {
    move->strafe_left(rand() % 360);
    move->climb_up(Level::get_bottom_of_level());
  }
}


void
Enemy_controller::on_end()
{
}


void
Enemy_controller::on_early_update(const float dt)
{
  switch(m_state)
  {
    case(State::up_down_search):
    {
      Move_model *move = get_entity().get_component<Move_model>(Component_type::move_model);
      assert(move);
      
      if(move)
      {
        const float depth = move->get_climb();
      
        if(depth <= Level::get_bottom_of_level() ||
           depth >= Level::get_top_of_level())
        {
          m_direction *= -1;
        }
      
        const float move_delta = static_cast<float>(m_direction * m_climb_speed) * dt;
        
        move->climb_up(move_delta);
      }
    }
  }
}


void
Enemy_controller::on_collision(const Core::Entity &collided_with)
{
  if(collided_with.has_tag(Tag::projectile))
  {
    get_entity().destroy();
    
    const Core::Transform trans = get_entity().get_transform();
    
    const Move_model *move = get_entity().get_component<Move_model>(Component_type::move_model);
    
    float strafe = 0.f;
    
    if(move)
    {
      strafe = move->get_strafe();
    }
    
    Entity_factory::create_explosion(m_world_data, trans);
    Entity_factory::create_multiplier(m_world_data, strafe);
  }
}