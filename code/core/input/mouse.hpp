#ifndef MOUSE_INCLUDED_BE60F1A9_5E84_493A_B717_ECD4FAE44A58
#define MOUSE_INCLUDED_BE60F1A9_5E84_493A_B717_ECD4FAE44A58


#include <core/common/core_fwd.hpp>


namespace Core {


class Mouse final
{
public:

  explicit        Mouse();
  explicit        Mouse(Core::Context &ctx);
                  ~Mouse();
  
  bool            is_valid() const;
                  operator bool() const;
  
  Axis            get_delta_position() const;
  Axis            get_position() const;
  
  bool            is_button_down(const uint32_t mouse_buttons) const;
  bool            is_button_down_on_frame(const uint32_t mouse_buttons) const;
  bool            is_button_up(const uint32_t mouse_buttons) const;
  bool            is_button_up_on_frame(const uint32_t mouse_buttons) const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard