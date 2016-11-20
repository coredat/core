#include <core/debug/point_renderer.hpp>
#include <core/debug/line_renderer.hpp>
#include <core/color/color.hpp>
#include <core/color/color_utils.hpp>
#include <core/color/color_predefined.hpp>
#include <core/transform/transform.hpp>
#include <core/transform/transform_utils.hpp>
#include <renderer/debug_line.hpp>
#include <math/vec/vec3.hpp>


namespace Core {
namespace Debug {


void
draw_axis_cross(const math::vec3 pos,
                const float size)
{
  const float half_size = size * 0.5f;

  // Left axis
  {
    const math::vec3 start = math::vec3_subtract(
      pos,
      math::vec3_scale(
        Core::Transform_utils::get_world_left(),
        half_size
      )
    );
    
    const math::vec3 end = math::vec3_add(
      pos,
      math::vec3_scale(
        Core::Transform_utils::get_world_left(),
        half_size
      )
    );
    
    Core::Debug::debug_line(start, end, Core::Color_utils::green());
  }

  // Up axis
  {
    const math::vec3 start = math::vec3_subtract(
      pos,
      math::vec3_scale(
        Core::Transform_utils::get_world_up(),
        half_size
      )
    );

    const math::vec3 end = math::vec3_add(
      pos,
      math::vec3_scale(
        Core::Transform_utils::get_world_up(),
        half_size
      )
    );
    
    Core::Debug::debug_line(start, end, Core::Color_utils::blue());
  }
  
  // Forward axis
  {
    const math::vec3 start = math::vec3_subtract(
      pos,
      math::vec3_scale(
        Core::Transform_utils::get_world_forward(),
        half_size
      )
    );

    const math::vec3 end = math::vec3_add(
      pos,
      math::vec3_scale(
        Core::Transform_utils::get_world_forward(),
        half_size
      )
    );
    
    Core::Debug::debug_line(start, end, Core::Color_utils::red());
  }
}



} // ns
} // ns