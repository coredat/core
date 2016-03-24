#ifndef COLOR_INCLUDED_9993A97E_9C18_4A52_AEB7_2004CD3A48E5
#define COLOR_INCLUDED_9993A97E_9C18_4A52_AEB7_2004CD3A48E5


#include <core/color/color.hpp>

/*
  Predefined colors.
*/
namespace Color_utils {


constexpr Color        white()      { return Color(0xFFFFFFFF); }
constexpr Color        black()      { return Color(0xFF000000); }
constexpr Color        red()        { return Color(0xFFFF0000); }
constexpr Color        green()      { return Color(0xFF00FF00); }
constexpr Color        blue()       { return Color(0xFF0000FF); }
constexpr Color        yellow()     { return Color(0xFFFFFF00); }
constexpr Color        magenta()    { return Color(0xFFFF00FF); }
constexpr Color        cyan()       { return Color(0xFF00FFFF); }
constexpr Color        null_color() { return Color(0x00000000); }


} // ns


#endif // inc guard