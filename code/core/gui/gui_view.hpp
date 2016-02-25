#ifndef GUI_VIEW_INCLUDED_608734C1_328F_40B8_929A_F0437981F0AA
#define GUI_VIEW_INCLUDED_608734C1_328F_40B8_929A_F0437981F0AA


#include <stdint.h>
#include <core/color/color.hpp>


namespace Core {


class Rect
{
  float top, bot, left, right;
};


class Gui_view
{
public:

  explicit          Gui_view();

  void              set_envelope(const Rect rect);
  void              set_color(const Color color);
  
private:

  uint32_t          m_gui_id;

}; // class


} // ns


#endif // inc guard