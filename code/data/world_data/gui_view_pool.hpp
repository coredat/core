#ifndef GUI_POOL_INCLUDED_91292FD0_1FC1_4F79_AD28_BE48370DE608
#define GUI_POOL_INCLUDED_91292FD0_1FC1_4F79_AD28_BE48370DE608


#include <stdint.h>
#include <renderer/gui_renderer/gui_node.hpp>


#define MAX_GUI_SIZE 32


namespace World_data {


struct Gui_view_pool
{
  uint32_t              id[MAX_GUI_SIZE];
  Gui_renderer::Node    gui[MAX_GUI_SIZE];
  
  const uint32_t size = MAX_GUI_SIZE;
}; // struct


void
gui_view_pool_init(Gui_view_pool *pool);


bool
gui_view_pool_add(Gui_view_pool *pool, const uint32_t id);


void
gui_view_pool_delete(Gui_view_pool *pool, const uint32_t id);


void
gui_view_pool_get(const Gui_view_pool *pool, const uint32_t id, Gui_renderer::Node *out_view);


void
gui_view_pool_set(const Gui_view_pool *pool, const uint32_t id, const Gui_renderer::Node *udpate_view);


} // ns


#endif // inc guard