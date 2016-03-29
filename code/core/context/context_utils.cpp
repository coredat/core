#include <core/context/context_utils.hpp>
#include <core/context/context_graphics_api.hpp>
#include <core/context/context.hpp>


namespace Core {
namespace Context_utils {


Core::Context
create_window(const Graphics_api gfx_api,
              const uint32_t width,
              const uint32_t height,
              const bool is_fullscreen,
              const std::string title)
{
  return Core::Context(gfx_api, width, height);
}


std::string
get_title(const Core::Context &context)
{
  return std::string(context.get_title());
}



} // ns
} // ns