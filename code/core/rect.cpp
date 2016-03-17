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


namespace Rect_utils
{

inline float
get_width(const Rect rect)
{
  return rect.get_right() - rect.get_left();
}


inline float
get_height(const Rect rect)
{
  return rect.get_bottom() - rect.get_top();
}


inline float
get_area(const Rect rect)
{
  return get_width(rect) * get_height(rect);
}

} // ns


} // ns