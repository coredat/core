#ifndef KEYBOARD_INCLUDED_D9A5F815_B560_45F6_83D4_C1ACBBDC26BD
#define KEYBOARD_INCLUDED_D9A5F815_B560_45F6_83D4_C1ACBBDC26BD


#include <core/common/core_fwd.hpp>


namespace Core {


class Keyboard
{
public:

  explicit          Keyboard();
  explicit          Keyboard(Core::Context &ctx);
  
  bool              is_valid() const;
                    operator bool() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard