#include "rect_util.hpp"


namespace Core {
namespace Rect_util {


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