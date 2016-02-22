#ifndef GUN_MODEL_INCLUDED_2C79D9CC_206D_41FC_A9A7_3178C8D57DCD
#define GUN_MODEL_INCLUDED_2C79D9CC_206D_41FC_A9A7_3178C8D57DCD


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>
#include <utils/timer.hpp>


/*!
  Gun_model only tracks the state of the gun,
  and the properties of its firing, such as spread.
  
  It doens't handle sending rays etc.
*/
class Gun_model : public Core::Component
{
public:
  
  uint32_t              get_rtti() const override { return Component_type::gun; }
  
  void                  on_update(const float dt) override;
  
  bool                  fire_gun();     //! Returns true if the gun state changed.
  bool                  reload();       //! Returns true if the gun state changed.
  
  bool                  is_reloading() const { return m_gun_state == State::reloading; }
  bool                  is_firing() const    { return m_gun_state == State::firing;    }
  
private:

  enum class State
  {
    ready,
    fired,
    firing,
    reloading,
    empty,
  };

  State                 m_gun_state       = State::ready;
  float                 m_reload_time     = 1050;
  float                 m_fire_time       = 85;
  uint32_t              m_bullets         = 200;
  uint32_t              m_loaded_bullets  = 10;
  uint32_t              m_max_clip_size   = 10;
  util::timer           m_timer;

}; // class


#endif // inc guard