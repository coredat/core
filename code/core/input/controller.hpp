#ifndef CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399
#define CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399


#include "detail/controller_members.hpp"
#include <stdint.h>


namespace Core {
namespace Input {


class Controller
{
public:

  explicit          Controller();

  Axis              get_axis(const uint8_t axis) const;
  bool              is_button_down(const Button::ENUM button) const;
  void              serialize(uint8_t out_data[16]);


private:

  INPUT_CONTROLLER_MEMBERS
  
}; // classs


} // ns
} // ns


#endif // inc guard
