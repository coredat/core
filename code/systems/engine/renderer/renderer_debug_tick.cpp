#include "renderer_debug_tick.hpp"
#include <graphics_api/initialize.hpp>


namespace
{
  constexpr nk_flags win_flags_debug    = NK_WINDOW_BORDER;
  constexpr nk_flags win_flag_inspector = NK_WINDOW_BORDER | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE;
}


namespace Engine {
namespace Renderer_debug_tick {


void
initialize()
{

}


void
think(std::shared_ptr<Data::World> world,
      std::shared_ptr<Data::Context> resources,
      nk_context *ctx,
      const float dt,
      const float running_time,
      const uint32_t viewport_width,
      const uint32_t viewport_height)
{
  return;

  const float screen_height = viewport_height;
  constexpr float window_spaceing = -2;
  
  float cursor = 0;
  
  // Data window
  {
    const float window_width = 400;
    struct nk_rect window_rect{cursor, 0, window_width, screen_height};
    cursor += window_width + window_spaceing;
    
    struct nk_panel layout;
  
    if(nk_begin(ctx, &layout, "Data", window_rect, win_flags_debug))
    {
      // Move cursor along.
      {
        struct nk_panel menu;
        nk_menubar_begin(ctx);
        
        nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
        nk_layout_row_push(ctx, 100);
        if (nk_menu_begin_label(ctx, &menu, "Context Data", NK_TEXT_LEFT, nk_vec2(120, 200)))
        {
            static size_t prog = 40;
            static int slider = 10;
            static int check = nk_true;
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_menu_item_label(ctx, "Hide", NK_TEXT_LEFT)) {}
    
            if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT)) {}

            nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
            nk_slider_int(ctx, 0, &slider, 16, 1);
            nk_checkbox_label(ctx, "check", &check);
            nk_menu_end(ctx);
        }
        
        if (nk_menu_begin_label(ctx, &menu, "World Data", NK_TEXT_LEFT, nk_vec2(320, 200)))
        {
            static size_t prog = 40;
            static int slider = 10;
            static int check = nk_true;
            nk_layout_row_dynamic(ctx, 100, 1);
            if (nk_menu_item_label(ctx, "Hide", NK_TEXT_LEFT)) {}
    
            if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT)) {}

            nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
            nk_slider_int(ctx, 0, &slider, 16, 1);
            nk_checkbox_label(ctx, "check", &check);
            nk_menu_end(ctx);
        }
        
        if (nk_menu_begin_label(ctx, &menu, "Foo Data", NK_TEXT_LEFT, nk_vec2(620, 200)))
        {
            static size_t prog = 40;
            static int slider = 10;
            static int check = nk_true;
            nk_layout_row_dynamic(ctx, 100, 1);
            if (nk_menu_item_label(ctx, "Hide", NK_TEXT_LEFT)) {}
    
            if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT)) {}

            nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
            nk_slider_int(ctx, 0, &slider, 16, 1);
            nk_checkbox_label(ctx, "check", &check);
            nk_menu_end(ctx);
        }
      }
      
      if (nk_tree_push(ctx, NK_TREE_TAB, "Context Data", NK_MINIMIZED))
      {
        if (nk_button_label(ctx, "show input data"))
            fprintf(stdout, "button pressed\n");

        if (nk_button_label(ctx, "show texture data"))
            fprintf(stdout, "button pressed\n");

        if (nk_button_label(ctx, "show shader data"))
            fprintf(stdout, "button pressed\n");

        
        nk_tree_pop(ctx);
      }
      
      if (nk_tree_push(ctx, NK_TREE_TAB, "World Data", NK_MINIMIZED))
      {
        if (nk_tree_push(ctx, NK_TREE_NODE, "Entity Data", NK_MINIMIZED))
        {
        
        
          nk_tree_pop(ctx);
        }
        
        nk_tree_pop(ctx);
      }
    
      enum {EASY, HARD};
      static int op = EASY;
      static int property = 20;

      nk_layout_row_static(ctx, 30, 80, 1);
      if (nk_button_label(ctx, "button"))
          fprintf(stdout, "button pressed\n");
      nk_layout_row_dynamic(ctx, 30, 2);
      if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
      if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
      nk_layout_row_dynamic(ctx, 25, 1);
      nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(ctx);
  }
  
  // Entity window
  {
    const float window_width = 200;
    struct nk_rect window_rect{cursor, 0, window_width, screen_height};
    cursor += window_width + window_spaceing;
    
    struct nk_panel layout;
    
    if(nk_begin(ctx, &layout, "Entity List", window_rect, win_flag_inspector))
    {
      auto win = nk_window_find(ctx, "Entity List");

      enum {EASY, HARD};
      static int op = EASY;
      static int property = 20;

      nk_layout_row_static(ctx, 30, 80, 1);
      if (nk_button_label(ctx, "button"))
          fprintf(stdout, "button pressed\n");
      nk_layout_row_dynamic(ctx, 30, 2);
      if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
      if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
      nk_layout_row_dynamic(ctx, 25, 1);
      nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(ctx);
    
    if(nk_window_is_closed(ctx, "Entity List"))
    {
      nk_window_find(ctx, "Entity List")->flags &= ~NK_WINDOW_CLOSED;
      nk_window_show(ctx, "Entity List", NK_SHOWN);
    }
  }
  
  // Entity
  {
    const float window_width = 200;
    struct nk_rect window_rect{cursor, 0, window_width, screen_height};
    cursor += window_width + window_spaceing;
    
    struct nk_panel layout;
    
    if(nk_begin(ctx, &layout, "Entity Inspector", window_rect, win_flag_inspector))
    {

      enum {EASY, HARD};
      static int op = EASY;
      static int property = 20;

      nk_layout_row_static(ctx, 30, 80, 1);
      if (nk_button_label(ctx, "button"))
          fprintf(stdout, "button pressed\n");
      nk_layout_row_dynamic(ctx, 30, 2);
      if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
      if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
      nk_layout_row_dynamic(ctx, 25, 1);
      nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(ctx);
    
    if(nk_window_is_closed(ctx, "Entity Inspector"))
    {
      nk_window_find(ctx, "Entity Inspector")->flags &= ~NK_WINDOW_CLOSED;
      nk_window_show(ctx, "Entity Inspector", NK_SHOWN);
    }
  }
}


void
de_initialize()
{
}


} // ns
} // ns