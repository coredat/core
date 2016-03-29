#ifndef CONTEXT_UTIL_INCLUDED_B76CA6DE_381A_4F68_8D36_4CCC0675AD01
#define CONTEXT_UTIL_INCLUDED_B76CA6DE_381A_4F68_8D36_4CCC0675AD01


#include <core/context/context_fwd.hpp>
#include <stdint.h>
#include <string>


namespace Core {
namespace Context_utils {


/*!
  Sugar method that accepts a STL string as input.
*/
Core::Context
create_window(const uint32_t width,
              const uint32_t height,
              const bool is_fullscreen = false,
              const std::string title = "");


/*!
  Sugar method that returns an STL string for a title.
*/
std::string
get_title(const Core::Context &context);


/*!
  Sugar method to set the resolution of the window.
  Internally just calls the the set methods for width etc.
*/
void
set_resolution(Core::Context &context,
               const uint32_t width,
               const uint32_t height,
               const bool is_fullscreen);


} // ns
} // ns


#endif // inc guard