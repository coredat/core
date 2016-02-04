#ifndef CORE_DATA_INCLDUED_3B7A1761_B49B_41D6_B7D9_7439A356614E
#define CORE_DATA_INCLDUED_3B7A1761_B49B_41D6_B7D9_7439A356614E


#include <core/input/axis.hpp>
#include <core/input/buttons.hpp>


namespace Core_data {

struct Input_data
{
  // Only suiteable for one player right now.
  ::Core::Input::Axis axis[2];
  ::Core::Input::Button_state buttons[16];
}; // struct



struct Core
{
  Input_data          *input_data;
}; // struct


Input_data*
get_input_data(Core_data::Core *core_data);


} // ns



#endif // inc gaurd