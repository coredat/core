#ifndef CONTROLLER_MEMBERS_INCLUDED_EE7B41B6_627C_4D4B_87A1_FA67F472B999
#define CONTROLLER_MEMBERS_INCLUDED_EE7B41B6_627C_4D4B_87A1_FA67F472B999


#include "../buttons.hpp"
#include "../axis.hpp"


#define INPUT_CONTROLLER_MEMBERS        \
  ::Core::Axis m_axis[2];               \
  ::Core::Button_state m_buttons[16];   \


#endif // inc guard