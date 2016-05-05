#ifndef COLOR_INCLUDED_9993A97E_9C18_4A52_AEB7_2004CD3A48E5
#define COLOR_INCLUDED_9993A97E_9C18_4A52_AEB7_2004CD3A48E5


#include <core/color/color.hpp>


/*
  Predefined colors.
*/
namespace Core {
namespace Color_utils {


constexpr Color        white()      { return Color(0xFFFFFFFF); }
constexpr Color        gray()       { return Color(0x808080FF); }
constexpr Color        black()      { return Color(0x000000FF); }

constexpr Color        red()        { return Color(0xFF0000FF); }
constexpr Color        green()      { return Color(0x00FF00FF); }
constexpr Color        blue()       { return Color(0x0000FFFF); }

constexpr Color        cyan()       { return Color(0x00FFFFFF); }
constexpr Color        yellow()     { return Color(0xFFFF00FF); }
constexpr Color        orange()     { return Color(0xFF8000FF); }
constexpr Color        magenta()    { return Color(0xFF00FFFF); }
constexpr Color        pink()       { return Color(0xFF0080FF); }
constexpr Color        purple()     { return Color(0x800080FF); }

constexpr Color        invisible()  { return Color(0x00000000); }


} // ns
} // ns


#endif // inc guard