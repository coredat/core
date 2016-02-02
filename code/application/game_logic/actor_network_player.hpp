#ifndef ACTOR_NETWORK_PLAYER_INCLUDED_0331D020_FA3D_4C41_85C6_42CE25D21639
#define ACTOR_NETWORK_PLAYER_INCLUDED_0331D020_FA3D_4C41_85C6_42CE25D21639


#include "actor_base.hpp"
#include <application/common/ids_component_types.hpp>


class Actor_network_player : public Actor_base
{
public:

  Actor_network_player();
  
  uint32_t get_rtti() const override { return Component_type::actor_network; }

  /** Hooks **/
  void on_update(const float dt) override;
};



#endif // inc guard