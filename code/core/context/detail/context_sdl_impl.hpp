#ifndef CONTEXT_SDL_IMPL_INCLUDED_CBE4657B_97B4_49A6_BA43_7394F39F07E4
#define CONTEXT_SDL_IMPL_INCLUDED_CBE4657B_97B4_49A6_BA43_7394F39F07E4


#include <core/common/core_fwd.hpp>
#include <core/context/detail/context_detail_fwd.hpp>
#include <systems/sdl_backend/sdl_message_loop.hpp>
#include <systems/sdl_backend/sdl_input.hpp>


namespace Core {
namespace Detail {


class Sdl_context
{
public:

  explicit        Sdl_context();
  explicit        Sdl_context(const uint32_t width,
                              const uint32_t height,
                              const bool is_fullscreen,
                              const char *title,
                              const Core::Context_setup &settings,
                              std::shared_ptr<Core::Context_detail::Data> data);
                  ~Sdl_context();
  
  bool            is_open() const;
  void            close();
  
  void            set_resolution(const uint32_t width,
                                 const uint32_t height);
  
  uint32_t        get_width() const;
  uint32_t        get_height() const;
  
  void            set_fullscreen(const bool set);
  bool            is_fullscreen() const;
  
  uint32_t        get_display_index() const;
  
  const char *    get_title() const;
  void            set_title(const char *title);
  
  bool            process();
  
  SDL_Window*     get_sdl_window() const;
  SDL_GLContext   get_sdl_gl_context() const;

private:
  bool                                         m_is_open = false;
  SDL_Window                                   *m_window = nullptr;
  SDL_GLContext                                m_context = nullptr;
  std::shared_ptr<Core::Context_detail::Data>  m_data    = nullptr;
};


} // ns
} // ns


#endif // inc guard