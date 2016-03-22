#include "multiplier_controller.hpp"
#include "move_model.hpp"
#include "../common/ids_component_types.hpp"


void
Multiplier_controller::on_start()
{
  Move_model *move = get_entity().get_component<Move_model>(Component_type::move_model);
  assert(move);
  
  if(move)
  {
    // Set direction.
  }
}


void
Multiplier_controller::on_collision(const Core::Entity &actor)
{
}