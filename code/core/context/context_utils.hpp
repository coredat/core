#ifndef CONTEXT_UTIL_INCLUDED_B76CA6DE_381A_4F68_8D36_4CCC0675AD01
#define CONTEXT_UTIL_INCLUDED_B76CA6DE_381A_4F68_8D36_4CCC0675AD01


#include <core/context/context_fwd.hpp>
#include <stdint.h>
#include <string>


namespace Core {
namespace Context_utils {


Core::Context     create_window(const Graphics_api gfx_api,
                                const uint32_t width,
                                const uint32_t height,
                                const bool is_fullscreen = false,
                                const std::string title = "");

std::string       get_title(const Core::Context &context);


} // ns
} // ns


#endif // inc guard