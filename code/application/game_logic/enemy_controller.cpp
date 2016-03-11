#include "enemy_controller.hpp"
#include <core/interface/entity.hpp>
#include "../common/ids_object_tags.hpp"


void
Enemy_controller::on_start()
{
}


void
Enemy_controller::on_end()
{
}


void
Enemy_controller::on_early_update(const float dt)
{

}


void
Enemy_controller::on_collision(const Core::Entity &collided_with)
{
  if(collided_with.has_tag(Tag::projectile))
  {
    get_entity().destroy();
  }
}