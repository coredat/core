#ifndef WINDOW_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763
#define WINDOW_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763


#include <stdint.h>


namespace Core {


class Window
{
public:

  explicit            Window(const uint32_t width,
                             const uint32_t height,
                             const bool is_fullscreen = false,
                             const char *title = "");
  
  uint32_t            get_width() const;
  uint32_t            get_height() const;
  bool                is_fullscreen() const;
  const char *        get_title() const;
  
private:

  

}; // class


} // ns


#endif // inc guard