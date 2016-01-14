#ifndef ACTOR_NETWORK_PLAYER_INCLUDED_0331D020_FA3D_4C41_85C6_42CE25D21639
#define ACTOR_NETWORK_PLAYER_INCLUDED_0331D020_FA3D_4C41_85C6_42CE25D21639


#include <systems/logic/logic_base.hpp>


class Actor_network_player : public Logic::Base
{
public:

  Actor_network_player();

  /** Hooks **/
  void on_start() override { std::cout << "net player_start" << std::endl; }
  void on_update(const float dt) override;

};



#endif // inc guard