#ifndef RECT_UTIL_INCLUDED_58078434_1328_4AA5_93F5_D29B03E27542
#define RECT_UTIL_INCLUDED_58078434_1328_4AA5_93F5_D29B03E27542


#include "rect.hpp"


namespace Core {
namespace Rect_util {


/*!
  Get the width of a rect.
*/
float
get_width(const Rect rect);


/*!
  Get the height of a rect.
*/
float
get_height(const Rect rect);


/*!
  Get the area of a rect.
*/
float
get_area(const Rect rect);


} // ns
} // ns


#endif // inc guard