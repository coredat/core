#ifndef EXPLOSION_MODEL_INCLUDED_D837CA0B_9751_409B_9585_4CE65AEC378E
#define EXPLOSION_MODEL_INCLUDED_D837CA0B_9751_409B_9585_4CE65AEC378E


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>


class Explosion_model : public Core::Component
{
private:

  uint32_t          get_rtti() const override { return Component_type::explosion_model; }
  COMPONENT_RTTI(Component_type::explosion_model)
  
  void              on_update(const float dt) override;
  
private:

  float             m_time = 0.1f;

}; // class


#endif // inc guard