#ifndef ACTOR_NETWORK_PLAYER_INCLUDED_0331D020_FA3D_4C41_85C6_42CE25D21639
#define ACTOR_NETWORK_PLAYER_INCLUDED_0331D020_FA3D_4C41_85C6_42CE25D21639


#include <application/ids_component_types.hpp>
#include <core/interface/component.hpp>
#include <core/interface/entity.hpp>


class Actor_network_player : public Core::Component
{
public:

  Actor_network_player();
  
  uint32_t get_rtti() const override { return Component_type::actor_network; }

  /** Hooks **/
  void on_start() override {}
  void on_update(const float dt) override;

};



#endif // inc guard