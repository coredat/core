#ifndef SDL_INPUT_INCLUDED_46AF05A3_F13C_4CDB_B560_75EFEC2B20B8
#define SDL_INPUT_INCLUDED_46AF05A3_F13C_4CDB_B560_75EFEC2B20B8


#include <data/context_data/input_pool.hpp>


namespace Sdl {


void update_gamepad_controller(Context_data::Game_controller *controller, const uint32_t controller_id);
void update_keyboard_controller(Context_data::Game_controller *controller);
void capture_mouse(const bool set);
bool is_mouse_captured();

} // ns


#endif // inc guard