#ifndef ACTOR_NETWORK_PLAYER_INCLUDED_
#define ACTOR_NETWORK_PLAYER_INCLUDED_


#include <systems/logic/logic_base.hpp>


class Actor_network_player : public Logic::Base
{
public:

  Actor_network_player();

  void on_start() override { std::cout << "net player_start" << std::endl; }
  
  void on_update(const float dt) override;

};



#endif // inc guard