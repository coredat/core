#ifndef INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76
#define INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76


#include <core/input/controller.hpp>
#include <stdint.h>


namespace Core {
namespace Input {


namespace Player {
enum ENUM {

  one = 0,
  two,
  three,
  four,

}; // enum
} // ns


Controller
get_controller(const Context &ctx, const Player::ENUM player);


Controller
get_controller(const Player::ENUM player);


} // ns
} // ns


#endif // inc guard
