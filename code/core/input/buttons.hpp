#ifndef BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93
#define BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93


namespace Core {
namespace Input {


namespace Button {
enum ENUM {

  button_0 = 0,
  button_1,
  button_2,
  button_3,
  button_4,
  button_5,
  button_6,
  button_7,
  

}; // enum
} // ns


enum class Button_state {

  up,
  down,
  
  up_on_frame,
  down_on_frame,

}; // enum


} // ns
} // ns


#endif // inc guard
