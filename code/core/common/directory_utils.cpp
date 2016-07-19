#include <core/common/directory_utils.hpp>
#include <core/common/directory.hpp>


namespace Core {
namespace Directory_utils {


std::string
resource_path_as_std_string(const std::string &to_append)
{
  return std::string(Directory::volatile_resource_path(to_append.c_str()));
}


} // ns
} // ns