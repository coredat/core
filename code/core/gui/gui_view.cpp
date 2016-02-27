#include "gui_view.hpp"
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/gui_view_pool.hpp>
#include <atomic>


namespace
{
  std::atomic<uint32_t> unique_view_id(0);
}


namespace Core {


Gui_view::Gui_view()
: m_gui_id(++unique_view_id)
{
  // Allocate into data.
  World_data::World *w_data = World_data::get_world();
  World_data::gui_view_pool_add(w_data->gui_pool, m_gui_id);
}


Gui_view::~Gui_view()
{
  // De allocdata.
  World_data::World *w_data = World_data::get_world();
  World_data::gui_view_pool_delete(w_data->gui_pool, m_gui_id);
}


void
Gui_view::set_envelope(const Rect set_rect)
{
  World_data::World *w_data = World_data::get_world();

  if(w_data)
  {
    Gui_renderer::Node gui_view;
    World_data::gui_view_pool_get(w_data->gui_pool, m_gui_id, &gui_view);
    
    gui_view.quad_env[0] = set_rect.get_left();
    gui_view.quad_env[1] = set_rect.get_top();
    gui_view.quad_env[2] = set_rect.get_right();
    gui_view.quad_env[3] = set_rect.get_bottom();
    
    World_data::gui_view_pool_set(w_data->gui_pool, m_gui_id, &gui_view);
  }
}


void
Gui_view::set_color(const Color set_color)
{
  World_data::World *w_data = World_data::get_world();

  if(w_data)
  {
    Gui_renderer::Node gui_view;
    World_data::gui_view_pool_get(w_data->gui_pool, m_gui_id, &gui_view);
    
    gui_view.color[3] = set_color.get_red();
    gui_view.color[2] = set_color.get_green();
    gui_view.color[1] = set_color.get_blue();
    gui_view.color[0] = set_color.get_alpha();
    
    World_data::gui_view_pool_set(w_data->gui_pool, m_gui_id, &gui_view);
  }
}


} // ns