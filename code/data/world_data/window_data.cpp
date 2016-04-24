#include "window_data.hpp"
#include <cstring>



namespace World_data {


void
application_window_init(Application_window *app)
{
  app->width = 0;
  app->height = 0;
  app->is_fullscreen = false;
  memset((void*)&app->title[0], 0, sizeof(app->title));
}


void
application_window_create(Application_window *app,
                          const uint32_t width,
                          const uint32_t height,
                          const bool is_fullscreen,
                          const char *title)
{

}


void
application_window_close(Application_window *app)
{
}


bool
application_window_is_open(Application_window *app)
{
  return false;
}


} // ns