#ifndef BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93
#define BUTTONS_INCLUDED_E3F986FC_EBB0_4FDC_A26E_553D31961A93


#include <core/common/core_types.hpp>
#include <utilities/bits.hpp>


namespace Core {


namespace Button {
enum ENUM : uint32_t { 

  button_0 = BIT(0),
  button_1 = BIT(1),
  button_2 = BIT(2),
  button_3 = BIT(3),
  button_4 = BIT(4),
  button_5 = BIT(5),
  button_6 = BIT(6),
  button_7 = BIT(7),
  

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
