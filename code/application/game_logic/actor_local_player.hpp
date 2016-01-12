#ifndef ACTOR_PLAYER_INCLUDED_
#define ACTOR_PLAYER_INCLUDED_


#include <systems/logic/logic_base.hpp>


class Actor_local_player : public Logic::Base
{
public:

  explicit Actor_local_player();

  void on_start() override;
  
  void on_update(const float dt) override;
  
  Entity::Entity_id cube_id;

};



#endif // inc guard