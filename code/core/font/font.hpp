#ifndef FONT_INCLUDED_7DCC02FC_8E3A_42D3_AE89_006EADFA6B77
#define FONT_INCLUDED_7DCC02FC_8E3A_42D3_AE89_006EADFA6B77


#include <core/common/core_types.hpp>


namespace Core {


class Font
{
public:

  explicit          Font();
  explicit          Font(const char *filename);
  explicit          Font(const uint32_t id);
  
  uint32_t  get_id() const;
  
private:

  uint32_t m_font_id = 0;

};


} // ns


#endif // inc guard
