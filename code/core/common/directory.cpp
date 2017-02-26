#include <core/common/directory.hpp>
#include <core/common/core_types.hpp>
#include <data/memory/memory_data.hpp>
#include <utilities/directory.hpp>
#include <utilities/file.hpp>
#include <utilities/string.hpp>
#include <utilities/platform.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Core {
namespace Directory {


const char*
volatile_resource_path(const char *append_path)
{
  static_assert(CORE_MAX_FILE_PATH >= LIB_MAX_FILE_PATH_SIZE, "CORE_MAX_FILE_PATH needs to be at least as big as LIB_MAX_FILE_PATH_SIZE.");

  char *path = SCRATCH_ALLOC(char, LIB_MAX_FILE_PATH_SIZE);
  memset(path, 0, sizeof(char) * LIB_MAX_FILE_PATH_SIZE);
  
  LOG_TODO_ONCE("Stub while moving to new utils");
  
  strcat(path, lib::dir::exe_path());
  
  #ifdef LIB_PLATFORM_MAC
  strcat(path, "../Resources/");
  #endif
  
  strcat(path, append_path);
  
  return path;
}


void
resource_path(char *out_buffer)
{
  static_assert(CORE_MAX_FILE_PATH >= LIB_MAX_FILE_PATH_SIZE, "CORE_MAX_FILE_PATH needs to be at least as big as LIB_MAX_FILE_PATH_SIZE.");
  
  LOG_TODO_ONCE("Stub while moving to new utils");
  
  const char *volatile_path = volatile_resource_path("");
  
  strlcpy(out_buffer, volatile_path, LIB_MAX_FILE_PATH_SIZE);
}


} // ns
} // ns
