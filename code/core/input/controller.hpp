#ifndef CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399
#define CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399


#include "detail/controller_members.hpp"
#include <stdint.h>


namespace Core {
namespace Input {


class Controller
{
public:

  Axis get_axis(const uint8_t axis);
  bool is_button_down(const Button::ENUM button);

private:

  INPUT_CONTROLLER_MEMBERS
  
}; // classs


} // ns
} // ns


#endif // inc guard
