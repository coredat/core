#include "../common/ids_object_tags.hpp"
#include "actor_controller.hpp"
#include "bullet_controller.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
#include <math/general/general.hpp>
#include <iostream>
#include "entity_factory.hpp"
#include "move_model.hpp"
#include "gun_model.hpp"


void
Actor_controller::on_start()
{
  get_entity().add_tag(Tag::player);
}


void
Actor_controller::on_early_update(const float dt)
{
  const Core::Input::Controller controller = Core::Input::get_controller(Core::Input::Player::one);
  
  // Movement
  {
    Move_model *move = get_entity().get_component<Move_model>(Component_type::move_model);
    assert(move);
    
    if(move)
    {
      // Jumping
      if(controller.is_button_down(Core::Input::Button::button_2))
      {
        move->jump();
      }
      
      const float movement = controller.get_axis(0).x;
      
      if(movement != 0)
      {
        move->strafe_left(movement * dt);
      }
    }
  }
  
  // Shooting
  {
    Gun_model *gun = get_entity().get_component<Gun_model>(Component_type::gun_model);
    assert(gun);
    
    if(gun)
    {
      if(controller.is_button_down(Core::Input::Button::button_0))
      {
        gun->fire_gun();
      }
    }
  }
} // on_update


void
Actor_controller::on_collision(const Core::Entity &entity)
{
  if(entity.has_tag(Tag::npc))
  {
    get_entity().destroy();
    Entity_factory::create_explosion(m_world_data, get_entity().get_transform());
  }
}