#ifndef BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93
#define BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93


#include <core/common/core_types.hpp>
#include <utilities/bits.hpp>


namespace Core {


namespace Gamepad_button {
enum ENUM : uint32_t { 

  button_a              = BIT(0),
  button_b              = BIT(1),
  button_x              = BIT(2),
  button_y              = BIT(3),
  
  button_start          = BIT(4),
  button_back           = BIT(5),
  
  button_left_stick     = BIT(6),
  button_right_stick    = BIT(7),
  
  button_right_shoulder = BIT(8),
  button_left_shoulder  = BIT(9),
  
  button_dpad_up        = BIT(10),
  button_dpad_down      = BIT(11),
  button_dpad_left      = BIT(12),
  button_dpad_right     = BIT(13),

}; // enum
} // ns


namespace Mouse_button {
enum ENUM : uint32_t {

  left    = BIT(0),
  middle  = BIT(1),
  right   = BIT(2),
  extra_1 = BIT(3),
  extra_2 = BIT(4),
  
}; // enum
} // ns


enum class Button_state {

  up,
  down,
  
  up_on_frame,
  down_on_frame,

}; // enum


} // ns


#endif // inc guard
