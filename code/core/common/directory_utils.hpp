#ifndef DIRECTORY_UTILS_INCLUDED_1CFEE259_7AAE_4214_8DE2_E7A67485006F
#define DIRECTORY_UTILS_INCLUDED_1CFEE259_7AAE_4214_8DE2_E7A67485006F


#include <string>


namespace Core {
namespace Directory_utils {


/*
  Returns an stl string containing the resource path,
  this backs onto the functions inside directory.hpp.
*/
std::string
resource_path(const std::string &to_append);


} // ns
} // ns


#endif // inc gaurd