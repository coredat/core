#ifndef TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B
#define TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Text_renderer final
{
public:

  explicit            Text_renderer();
  explicit            Text_renderer(const util::generic_id font_id, const util::generic_id text_id);

  void                set_font(const Font &font);
  Font                get_font() const;

  void                set_text(const char *str);
  const char *        get_text() const;

                      operator Renderer() const;

//private:
//  
//  friend class Entity;
//  friend class Entity_ref;

  util::generic_id        get_font_id() const;
  util::generic_id        get_text_id() const;
  
private:

  util::generic_id m_font_id = util::generic_id_invalid();
  util::generic_id m_text_id = util::generic_id_invalid();
  
};


} // ns


#endif // inc guard