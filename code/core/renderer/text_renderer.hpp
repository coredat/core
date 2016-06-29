#ifndef TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B
#define TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B


#include <core/common/core_fwd.hpp>


namespace Core {


class Text_renderer final
{
public:

  explicit            Text_renderer();
  
  void                set_font(class Font &font);
  Font                get_font();
  
  void                set_text(const char *str);
  const char *        get_text() const;
  uint32_t            size_of_text() const;
  
private:

  
  
};


} // ns


#endif // inc guard