#ifndef FONT_INCLUDED_7DCC02FC_8E3A_42D3_AE89_006EADFA6B77
#define FONT_INCLUDED_7DCC02FC_8E3A_42D3_AE89_006EADFA6B77


#include <core/common/core_types.hpp>


namespace Core {


class Font
{
public:

  explicit          Font(const char *filename);
  
  util::generic_id  get_id() const;
  
private:

  util::generic_id m_font_id;

};


} // ns


#endif // inc guard