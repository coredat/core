#ifndef CONTEXT_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763
#define CONTEXT_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763


#include <core/context/context_fwd.hpp>
#include <stdint.h>
#include <memory>


namespace Core {


/*!
  Creates an application window or context.
  Note: There can only be one window per application.
  
  Rename to Context or App context
*/
class Context final
{

  Context(Context &) = delete;
  Context& operator=(Context&) = delete;

public:

  explicit            Context(const Graphics_api gfx_api,
                              const uint32_t width,
                              const uint32_t height,
                              const bool is_fullscreen = false,
                              const char *title = "");
  
                      ~Context();
  
                      Context(Context &&);
  Context&            operator=(Context&&);
  
  uint32_t            get_width() const;
  uint32_t            get_height() const;
  bool                is_fullscreen() const;
  const char *        get_title() const;
  
  
  bool                is_open() const;
                      operator bool() const; // Alias for is_open().
  
private:

  struct Impl;
  std::unique_ptr<Impl>   m_impl;

}; // class


} // ns


#endif // inc guard