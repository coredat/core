#include "multiplier_controller.hpp"
#include "move_model.hpp"
#include "../common/ids_component_types.hpp"
#include "../common/ids_object_tags.hpp"
#include <core/interface/entity.hpp>


void
Multiplier_controller::on_start()
{

}


void
Multiplier_controller::on_update(const float dt)
{
  Move_model *move = get_entity().get_component<Move_model>(Component_type::move_model);
  assert(move);
  
  if(move)
  {
    move->climb_up(dt * 3.f);
  }
}


void
Multiplier_controller::on_collision(const Core::Entity &entity)
{
  if(entity.has_tag(Tag::actor))
  {
    get_entity().destroy();
  }
}