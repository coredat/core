#ifndef CONTEXT_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763
#define CONTEXT_INCLUDED_7C70C70A_36FA_4BD0_AC6D_CA6E2F5BF763


#include <core/context/context_setup.hpp>
#include <core/context/detail/context_detail_fwd.hpp>
#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <core/input/input.hpp>


namespace Core {


/*
  Core Context represents the hardware in your application.
  Input, windows etc.
*/
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
  
  const char *        get_title() const;                        //!< Returns the title of the context.
  void                set_title(const char * title);            //!< Sets the title, if platform supports it, it will be displayed in the window.
  
  uint32_t            get_width() const;                        //!< Gets the current width of the graphics context.
  uint32_t            get_height() const;                       //!< Gets the height of the graphics context.
  void                set_resolution(const uint32_t width,
                                     const uint32_t height);    //!< Sets the context resolution.
  
  bool                is_fullscreen() const;                    //!< Returns true if the context is fullscreen.
  void                set_fullscreen(const bool fullscreen);    //!< Sets the context fullscreen or not if the platform supports windowing.
  
  uint32_t            get_display() const;                      //!< Returns the display number.
  
  bool                is_open() const;                          //!< Returns true if the context is open.
                      operator bool() const;                    //!< Shorthand for is_open().
  
  void                close();                                  //!< Closes the context. is_open will return false;
  
private:

  friend class Core::Controller;
  friend class Core::World;
  friend Axis Input::mouse_get_coordinates(Core::Context &context);
  friend bool Input::controllers_are_in_use(Core::Context &context);
  
  explicit            Context(std::shared_ptr<Context_detail::Data> data);

  std::shared_ptr<const Context_detail::Data>  get_context_data() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


} // ns


#endif // inc guard