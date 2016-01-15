#ifndef INPUT_INCLUDED_6BE027AB_AC40_4137_AE38_CB92A442027B
#define INPUT_INCLUDED_6BE027AB_AC40_4137_AE38_CB92A442027B


#include <functional>


namespace Environment {

namespace Button {
enum ENUM
{
  action_button,
  
  size
};
} // ns


enum class Button_action
{
  resting,
  on_up,
  on_down,
  held,
};


struct Controller
{
  uint8_t buttons[Button::size];

  float axis_1[2];
  float axis_2[2];
}; // struct



struct Input
{
  Controller controllers[4];
};



void
input_create(Input *input);


void
input_destroy(Input *input);


} // ns


#endif // inc guard