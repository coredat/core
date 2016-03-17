#ifndef GUI_VIEW_INCLUDED_608734C1_328F_40B8_929A_F0437981F0AA
#define GUI_VIEW_INCLUDED_608734C1_328F_40B8_929A_F0437981F0AA


#include <stdint.h>
#include <core/common/rect.hpp>
#include <core/color/color.hpp>


namespace Core {


/*!
  Represents an area on the GUI.
  This can be styled accordingly.
*/
class Gui_view final
{

  Gui_view(Gui_view &&)                 = delete;
  Gui_view(const Gui_view &)            = delete;
  Gui_view& operator=(Gui_view&&)       = delete;
  Gui_view& operator=(const Gui_view&)  = delete;

public:

  explicit          Gui_view();
                    ~Gui_view();

  void              set_envelope(const Rect rect);        // Sets the area
  void              set_color(const Color color);
  
private:

  uint32_t          m_gui_id;

}; // class


} // ns


#endif // inc guard