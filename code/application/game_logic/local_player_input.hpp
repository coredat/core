#ifndef LOCAL_PLAYER_INPUT_INCLUDED_AB2C94CC_3827_4B7B_81F1_66596B66ADD6
#define LOCAL_PLAYER_INPUT_INCLUDED_AB2C94CC_3827_4B7B_81F1_66596B66ADD6


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>


class Local_player_input : public Core::Component
{
public:

  explicit              Local_player_input();

  uint32_t              get_rtti() const override { return Component_type::local_player_input; }
  
  void                  on_update(const float dt) override;


}; // class


#endif // inc guard