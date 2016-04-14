#ifndef CONTEXT_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763
#define CONTEXT_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763


#include <core/context/context_fwd.hpp>
#include <core/context/context_setup.hpp>
#include <core/context/detail/context_detail_fwd.hpp>
#include <core/input/input_fwd.hpp>
#include <stdint.h>
#include <memory>


namespace Core {


class Context final
{
private:

                      Context(Context &) = delete;
                      Context& operator=(Context&) = delete;

public:

  explicit            Context(const uint32_t width,
                              const uint32_t height,
                              const bool is_fullscreen = false,
                              const char *title = "",
                              const Context_setup setup_options = Context_setup{});
  
                      ~Context();
  
                      Context(Context &&);
  Context&            operator=(Context&&);
  
  const char *        get_title() const;
  void                set_title(const char * title);
  
  uint32_t            get_width() const;
  void                set_width(const uint32_t width);
  
  uint32_t            get_height() const;
  void                set_height(const uint32_t height);
  
  bool                is_fullscreen() const;
  void                set_fullscreen(const bool fullscreen);
  
  bool                is_open() const;
                      operator bool() const; // Alias for is_open().
  
private:

  friend class Core::Input::Controller;

  std::shared_ptr<const Context_detail::Context_data>  get_context_data() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl>   m_impl;

}; // class


} // ns


#endif // inc guard