#ifndef TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B
#define TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Text_renderer final
{
public:

  explicit            Text_renderer();
  explicit            Text_renderer(const util::generic_id font_id,
                                    const util::generic_id text_id);
  
                      ~Text_renderer();

  void                set_font(const Font &font);
  Font                get_font() const;

  void                set_text(const char *str);
  const char *        get_text() const;
  
  void                set_text_size(const uint32_t size);
  uint32_t            get_text_size() const;

                      operator Renderer() const;

  util::generic_id    get_font_id() const;
  util::generic_id    get_text_id() const;
  
private:

  struct Impl;
  Impl *m_impl;

  util::generic_id    m_font_id = util::generic_id_invalid();
  util::generic_id    m_text_id = util::generic_id_invalid();
};


} // ns


#endif // inc guard