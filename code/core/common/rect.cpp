#include "rect.hpp"
#include <math/general/general.hpp>


namespace Core {


Rect::Rect()
: Rect(0,0,0,0)
{
}


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


} // ns