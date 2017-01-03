#include <core/font/text.hpp>


namespace Core {


Text::Text()
{
}


Text::Text(const char *)
{
}


const char *
Text::get_stripped_text() const
{
  return "";
}


const char *
Text::get_text() const
{
  return "";
}


const char *
Text::c_str_strip() const
{
  return get_stripped_text();
}


const char *
Text::c_str() const
{
  return get_text();
}




} // ns