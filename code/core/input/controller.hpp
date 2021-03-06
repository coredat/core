#ifndef CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399
#define CONTROLLER_INCLUDED_19652084_EF01_45A5_B098_E66B2AB39399


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Controller final
{
public:

  explicit          Controller();
  explicit          Controller(const Core::Context &ctx, const uint32_t controller_id);
                    ~Controller();
  
  bool              is_valid() const;
                    operator bool() const;
  
                    Controller(const Controller &);
  Controller&       operator=(const Controller &);

  Axis              get_axis(const uint8_t axis) const;                              //!< Get the axis of the controller. 0 or 1
  float             get_trigger(const uint8_t trigger) const;                        //!< Get the trigger of the controller. 0 or 1
  
  bool              is_button_down(const uint32_t gamepad_buttons) const;            //!< Returns true if the button is down. You can 'or' keys together
  bool              is_button_down_on_frame(const uint32_t gamepad_buttons) const;   //!< Returns true if the button is down on that frame. You can 'or' keys together
  
  bool              is_button_up(const uint32_t gamepad_buttons) const;              //!< Returns true if the button is up. You can 'or' keys together
  bool              is_button_up_on_frame(const uint32_t gamepad_buttons) const;     //!< Returns true if the button is up on that frame. You can 'or' keys together
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;
  
}; // classs


} // ns


#endif // inc guard
