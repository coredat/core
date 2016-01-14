#ifndef WINDOW_INCLUDED_39C03281_9ECB_430F_BB2C_FB77D86F0F51
#define WINDOW_INCLUDED_39C03281_9ECB_430F_BB2C_FB77D86F0F51


#include "environment_fwd.hpp"
#include <stdint.h>


namespace Environment {


void
window_create(Window *out_window, const uint32_t width, const uint32_t height, const bool fullscreen);


void
window_update(Window *out_window, const uint32_t width, const uint32_t height, const bool fullscreen);


void
window_destroy(Window *window);


uint32_t
window_get_width(const Window *window);


uint32_t
window_get_height(const Window *window);


void
window_callbacks(const Window *window, const std::function<void()> &callback);


}


#endif // inc guard