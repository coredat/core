#include <core/window/window_utils.hpp>


namespace Core {
namespace Window_utils {


Core::Window
create_window(const uint32_t width,
              const uint32_t height,
              const bool is_fullscreen,
              const std::string title)
{
  return Core::Window(width, height);
}


std::string
get_title(const Core::Window &window)
{
  return std::string(window.get_title());
}



} // ns
} // ns