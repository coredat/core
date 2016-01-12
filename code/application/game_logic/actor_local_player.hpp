#ifndef ACTOR_PLAYER_INCLUDED_
#define ACTOR_PLAYER_INCLUDED_


#include <systems/logic/logic_base.hpp>


class Actor_local_player : public Logic::Base
{
public:

  Actor_local_player();

  void on_start() override { std::cout << "player_start" << std::endl; }
  
  void on_update(const float dt) override;

};



#endif // inc guard