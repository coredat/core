#ifndef WINDOW_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763
#define WINDOW_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763


#include <stdint.h>
#include <memory>


namespace Core {


class Window final
{

  Window(Window &) = delete;
  Window& operator=(Window&) = delete;

public:

  Window(Window &&);
  Window& operator=(Window&&);

  explicit            Window(const uint32_t width,
                             const uint32_t height,
                             const bool is_fullscreen = false,
                             const char *title = "");
  
                      ~Window();
  
  uint32_t            get_width() const;
  uint32_t            get_height() const;
  bool                is_fullscreen() const;
  const char *        get_title() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl>   m_impl;

}; // class


} // ns


#endif // inc guard