#include <core/common/directory.hpp>
#include <core/common/core_types.hpp>
#include <data/global_data/memory_data.hpp>
#include <utilities/directory.hpp>
#include <utilities/file.hpp>


namespace Core {
namespace Directory {


const char*
resource_path(const char *url)
{
  char * path = SCRATCH_ALLOC(char, MAX_FILE_PATH_SIZE);
  memset(path, 0, MAX_FILE_PATH_SIZE);
  
  strcat(path, util::dir::resource_path());
  strcat(path, url);
  
  return path;
}


} // ns
} // ns