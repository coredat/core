#ifndef GUN_MODEL_INCLUDED_DE0ECD76_C3EE_429F_AFF9_04E59E809D8F
#define GUN_MODEL_INCLUDED_DE0ECD76_C3EE_429F_AFF9_04E59E809D8F


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>


class Gun_model : public Core::Component
{
private:

  uint32_t          get_rtti() const override { return Component_type::gun_model; }
  COMPONENT_RTTI(Component_type::gun_model)
  
  void              on_update(const float dt) override;
  
public:

  void              fire_gun();
  
private:

  enum class Gun_state
  {
    ready,
    fired,
    loading,
  };

  Gun_state         m_curr_state      = Gun_state::ready;
  float             m_curr_cooldown   = 0.f;
  float             m_max_cooldown    = 0.1f;

}; // class


#endif // inc guard