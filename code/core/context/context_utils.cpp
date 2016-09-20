#include <core/context/context_utils.hpp>
#include <core/context/context.hpp>


namespace Core {
namespace Context_utils {


Core::Context
create_window(const uint32_t width,
              const uint32_t height,
              const bool is_fullscreen,
              const std::string title)
{
  return Core::Context(width, height, is_fullscreen, title.c_str());
}


std::string
get_title(const Core::Context &context)
{
  return std::string(context.get_title());
}


void
set_resolution(Core::Context &context,
               const uint32_t width,
               const uint32_t height,
               const bool is_fullscreen)
{
  context.set_resolution(width, height);
  context.set_fullscreen(is_fullscreen);
}



} // ns
} // ns