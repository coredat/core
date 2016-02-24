#ifndef GAME_STATE_INCLUDED_E3AA3F00_B170_4CBA_A78D_E3E794DED583
#define GAME_STATE_INCLUDED_E3AA3F00_B170_4CBA_A78D_E3E794DED583


#include <core/interface/component.hpp>


class Game_state : public Core::Component
{
private:

  void            on_start() override;
  void            on_event(const uint32_t id, const void *data, uint32_t size_of_data) override;
  
public:

  enum class State
  {
    stealth,
    alarms,
  };
  
  
  State           m_game_state = State::stealth;

};


#endif // inc guard