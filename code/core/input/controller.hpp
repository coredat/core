#ifndef CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399
#define CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Controller final
{
public:

  explicit          Controller(const Core::Context &ctx, const uint32_t controller_id);
                    ~Controller();
  
                    Controller(const Controller &);
                    Controller(Controller &&);
  
  Controller&       operator=(const Controller &);
  Controller&       operator=(Controller &&);

  Axis              get_axis(const uint8_t axis) const;
  float             get_trigger(const uint8_t trigger) const;
  
  bool              is_button_down(const Button::ENUM button) const;
  bool              is_button_down_on_frame(const Button::ENUM button) const;
  
  bool              is_button_up(const Button::ENUM button) const;
  bool              is_button_up_on_frame(const Button::ENUM button) const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;
  
}; // classs


} // ns


#endif // inc guard
