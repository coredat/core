#ifndef WINDOW_INCLUDED_39C03281_9ECB_430F_BB2C_FB77D86F0F51
#define WINDOW_INCLUDED_39C03281_9ECB_430F_BB2C_FB77D86F0F51


#include "environment_fwd.hpp"
#include <functional>
#include <stdint.h>
#include <ostream>


namespace Environment {


bool
window_create(Window *out_window, const uint32_t width, const uint32_t height, const bool is_fullscreen, const char *title, std::ostream *log = nullptr);


bool
window_set_resolution(Window *out_window, const uint32_t width, const uint32_t height, std::ostream *log = nullptr);


bool
window_set_fullscreen(Window *out_window, const bool is_fullscreen, std::ostream *log = nullptr);


void
window_destroy(Window *window, std::ostream *log = nullptr);


uint32_t
window_get_width(const Window *window, std::ostream *log = nullptr);


uint32_t
window_get_height(const Window *window, std::ostream *log = nullptr);


void
window_think(const Window *window, std::ostream *log = nullptr);


void
window_callbacks(const Window *window, const std::function<void()> &callback, std::ostream *log = nullptr);


} // ns


#endif // inc guard