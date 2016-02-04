#ifndef BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93
#define BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93


namespace Core {
namespace Input {


namespace Button {
enum ENUM {

  button_0 = 1 << 0,
  button_1 = 1 << 1,
  button_2 = 1 << 2,
  button_3 = 1 << 3,
  button_4 = 1 << 4,
  button_5 = 1 << 5,
  button_6 = 1 << 6,
  button_7 = 1 << 7,

}; // enum
} // ns


enum class Button_state {

  button_up,
  button_down,

}; // enum


} // ns
} // ns


#endif // inc guard
