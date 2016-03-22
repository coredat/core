#include "enemy_controller.hpp"
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
      Core::Transform trans = get_entity().get_transform();
      float move = static_cast<float>(m_direction * m_climb_speed) * dt;
      
      const math::vec3 old_pos = trans.get_position();
      const math::vec3 new_pos = math::vec3_init(
        math::vec3_get_x(old_pos),
        math::vec3_get_y(old_pos),
        math::vec3_get_z(old_pos) + move
      );
      
      trans.set_position(new_pos);
      get_entity().set_transform(trans);
      
      if(math::vec3_get_z(new_pos) <= Level::get_bottom_of_level() || math::vec3_get_z(new_pos) >= Level::get_top_of_level())
      {
        m_direction *= -1;
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
    
    Entity_factory::create_explosion(m_world_data, trans);
    Entity_factory::create_multiplier(m_world_data, trans);
  }
}