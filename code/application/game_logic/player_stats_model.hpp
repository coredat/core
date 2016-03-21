#ifndef PLAYER_STATS_MODEL_INCLUDED_F42679E9_E687_4B33_9B33_0BEF317360C0
#define PLAYER_STATS_MODEL_INCLUDED_F42679E9_E687_4B33_9B33_0BEF317360C0


// TODO: Requires global events to pick on up stats.


#include <core/interface/component.hpp>
#include "../common/ids_component_types.hpp"
#include <stdint.h>


class Player_stats_model : public Core::Component
{
public:

  

private:


  uint32_t          get_rtti() const override { return Component_type::player_stats_model; }
  COMPONENT_RTTI(Component_type::player_stats_model)


private:

  enum Player // Maybe move this to common.
  {
    one = 0,
    two,
    three,
    four,
    
    count,
  };
  
  struct Player_stat
  {
    uint32_t kills = 0;
    uint32_t multiplier = 0;
    uint32_t wins = 0;
  };

  Player_stat         m_stats[Player::count];

}; // class


#endif // inc guard