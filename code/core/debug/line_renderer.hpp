#ifndef LINE_RENDERER_INCLUDED_CBEA6A96_79E5_4E33_9C60_12409DCBB84E
#define LINE_RENDERER_INCLUDED_CBEA6A96_79E5_4E33_9C60_12409DCBB84E


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {
namespace Debug {


/*!
  Draw a line with a color.
  This may perform subpar, its only meaent as an aide for debugging.
*/
void
debug_line(const math::vec3 start, const math::vec3 end, const Core::Color color);


void
debug_line(const Core::Ray ray, const Core::Color color);


} // ns
} // ns


#endif // inc guard