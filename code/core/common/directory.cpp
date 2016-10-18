#include <core/common/directory.hpp>
#include <core/common/core_types.hpp>
#include <data/memory/memory_data.hpp>
#include <utilities/directory.hpp>
#include <utilities/file.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>


namespace Core {
namespace Directory {


const char*
volatile_resource_path(const char *append_path)
{
  static_assert(CORE_MAX_FILE_PATH >= MAX_FILE_PATH_SIZE, "CORE_MAX_FILE_PATH needs to be at least as big as MAX_FILE_PATH_SIZE.");

  char *path = SCRATCH_ALLOC(char, MAX_FILE_PATH_SIZE);
  memset(path, 0, sizeof(char) * MAX_FILE_PATH_SIZE);
  
  strcat(path, util::dir::resource_path());
  strcat(path, append_path);
  
  return path;
}


void
resource_path(char *out_buffer)
{
  static_assert(CORE_MAX_FILE_PATH >= MAX_FILE_PATH_SIZE, "CORE_MAX_FILE_PATH needs to be at least as big as MAX_FILE_PATH_SIZE.");
  
  strlcpy(out_buffer, util::dir::resource_path(), MAX_FILE_PATH_SIZE);
}


} // ns
} // ns