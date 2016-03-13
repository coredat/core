#include "game_controller.hpp"
#include "../entity_factory.hpp"
#include <core/interface/entity.hpp>
#include "../level_functions.hpp"



void
Game_controller::on_start()
{
  Core::Entity e = Entity_factory::create_enemy(m_world_data);
  
  Core::Transform trans = e.get_transform();
  trans.set_position(math::vec3_init(0, 0, -10));
  e.set_transform(trans);
  e.set_material_id(1);
  
  e = Entity_factory::create_enemy(m_world_data);
  
  trans = e.get_transform();
  trans.set_position(math::vec3_init(0, 0, -10));
  e.set_transform(trans);
  e.set_material_id(2);
}


void
Game_controller::on_early_update(const float dt)
{

}