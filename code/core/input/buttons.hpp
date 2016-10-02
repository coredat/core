#ifndef BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93
#define BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93


#include <core/common/core_types.hpp>
#include <utilities/bits.hpp>


namespace Core {


namespace Gamepad_button {
enum ENUM : uint32_t { 

  a              = BIT(0),
  b              = BIT(1),
  x              = BIT(2),
  y              = BIT(3),
  
  start          = BIT(4),
  back           = BIT(5),
  
  left_stick     = BIT(6),
  right_stick    = BIT(7),
  
  right_shoulder = BIT(8),
  left_shoulder  = BIT(9),
  
  dpad_up        = BIT(10),
  dpad_down      = BIT(11),
  dpad_left      = BIT(12),
  dpad_right     = BIT(13),

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
