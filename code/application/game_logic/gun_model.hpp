#ifndef GUN_MODEL_INCLUDED_
#define GUN_MODEL_INCLUDED_


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>


class Gun_model : public Core::Component
{
public:

  explicit              Gun_model();
  
  uint32_t              get_rtti() const override { return Component_type::gun; }
  
  void                  on_update(const float dt) override;
  
  void                  fire_gun();
  void                  reload();
  
private:

  float                 m_reload_time = 10;
  float                 m_clip_size   = 10;
  float                 m_bullet_load = 1;

}; // class


#endif // inc guard