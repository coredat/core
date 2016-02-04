#ifndef INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76
#define INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76


#include "controller.hpp"
#include <stdint.h>


namespace Core {
namespace Input {


namespace Player {
enum ENUM {

  player_one,

}; // enum
} // ns


Controller
get_controller(const Player::ENUM player);


} // ns
} // ns


#endif // inc guard
