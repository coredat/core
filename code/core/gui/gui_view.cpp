#include "gui_view.hpp"
#include <atomic>


namespace
{
  std::atomic<uint32_t> unique_view_id(0);
}


namespace Core {


Gui_view::Gui_view()
{
  ++unique_view_id;
}


void
Gui_view::set_envelope(const Core::Rect set_rect)
{
}


void
Gui_view::set_color(const Color set_color)
{
}


} // ns