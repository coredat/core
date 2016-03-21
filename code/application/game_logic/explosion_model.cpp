#include "explosion_model.hpp"
#include <math/general/general.hpp>
#include <core/interface/component.hpp>
#include <core/transform/transform.hpp>
#include <core/interface/entity.hpp>


void
Explosion_model::on_update(const float dt)
{
  m_time += (dt * 3.f);
  
  const float scale = math::cos(m_time) * 2.f;
  
  if(scale > 0.f)
  {
    const math::vec3 obj_scale = math::vec3_init(scale);
    
    Core::Transform curr_trans = get_entity().get_transform();
    curr_trans.set_scale(obj_scale);
    
    get_entity().set_transform(curr_trans);
  }
  else
  {
    get_entity().destroy();
  }
}