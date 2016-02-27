#include "rect.hpp"
#include <math/general/general.hpp>


namespace Core {


Rect::Rect(const float top,
           const float left,
           const float bottom,
           const float right)
: m_top(math::max(top, bottom))
, m_bottom(math::min(top, bottom))
, m_left(math::min(left, right))
, m_right(math::max(left, right))
{
}


float
Rect::get_area() const
{
  const float height = m_bottom - m_top;
  const float width = m_right - m_left;
  
  return height * width;
}


} // ns