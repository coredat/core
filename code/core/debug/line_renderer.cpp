#include <core/debug/line_renderer.hpp>
#include <core/color/color.hpp>
#include <core/color/color_utils.hpp>
#include <renderer/debug_line.hpp>
#include <math/vec/vec3.hpp>


namespace Core {
namespace Debug {


void
debug_line(const math::vec3 start, const math::vec3 end, const Color color)
{
  ::Renderer::debug_line(start,
                         end,
                         math::vec3_init(Color_utils::get_red_f(color),
                                         Color_utils::get_green_f(color),
                                         Color_utils::get_blue_f(color))
                                         );
}



} // ns
} // ns