#ifndef DIRECTORY_INCLUDED_15B6ED5A_054E_4BB0_8B60_C0ABB2800A88
#define DIRECTORY_INCLUDED_15B6ED5A_054E_4BB0_8B60_C0ABB2800A88


#define CORE_MAX_FILE_PATH 2048


#include <stdarg.h>


namespace Core {
namespace Directory {


/*!
  Returns a string with the full filepath and the resource
  appended to the end. This return value is only valid for one frame.
  \param resource file or path to be appended to the resource directory.
  \return returns a path that is valid for one frame.
*/

const char*
volatile_resource_path(const char *append_path);

/*!
  Returns a string containing the resource path.
  make sure the buffer is at least 'CORE_MAX_FILE_PATH' big.
  \param out_buffer The output buffer.
*/
void
resource_path(char *out_buffer);


} // ns
} // ns


#endif // inc guard