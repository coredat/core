#ifndef WINDOW_UTIL_INCLUDED_B76CA6DE_381A_4F68_8D36_4CCC0675AD01
#define WINDOW_UTIL_INCLUDED_B76CA6DE_381A_4F68_8D36_4CCC0675AD01


#include <core/window/window.hpp>
#include <stdint.h>
#include <string>


namespace Core {
namespace Window_utils {


Core::Window      create_window(const uint32_t width,
                                const uint32_t height,
                                const bool is_fullscreen = false,
                                const std::string title = "");

std::string       get_title(const Core::Window &window);


} // ns
} // ns


#endif // inc guard