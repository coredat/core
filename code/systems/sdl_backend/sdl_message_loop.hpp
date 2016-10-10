#ifndef SDL_MESSAGE_LOOP_INCLUDED_2194BF40_5CC2_4B66_80A3_171A12F942B8
#define SDL_MESSAGE_LOOP_INCLUDED_2194BF40_5CC2_4B66_80A3_171A12F942B8


#include <SDL2/SDL.h>


namespace Sdl {


constexpr int context_slot() { return 0; }
constexpr int input_slot() { return 1; }


using Callback_event = void (*)(const SDL_Event *, void *self);


void event_add_callback(Callback_event cb, int slot, void *user_data);
void event_process();


} // ns


#endif // inc guard