#ifndef TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B
#define TEXT_RENDERER_INCLUDED_F5EE743F_A7E9_4CA1_BEC2_B0AB1994A96B


#include <core/common/core_fwd.hpp>


namespace Core {


class Text_renderer final
{
public:

  explicit            Text_renderer();
  explicit            Text_renderer(const Font &font, const char *str);
  
                      Text_renderer(const Text_renderer &other);
                      Text_renderer(Text_renderer &&other);
  
  Text_renderer&      operator=(const Text_renderer &other);
  Text_renderer&      operator=(Text_renderer &&other);
  
                      ~Text_renderer();

  void                set_font(const Font &font);
  Font                get_font() const;

  void                set_text(const char *str);
  const char *        get_text() const;
  
  void                set_text_size(const uint32_t size);
  uint32_t            get_text_size() const;

                      operator Renderer() const;
  
private:

  struct Impl;
  Impl *m_impl = nullptr;

};


} // ns


#endif // inc guard