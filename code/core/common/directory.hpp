#ifndef DIRECTORY_INCLUDED_15B6ED5A_054E_4BB0_8B60_C0ABB2800A88
#define DIRECTORY_INCLUDED_15B6ED5A_054E_4BB0_8B60_C0ABB2800A88


namespace Core {
namespace Directory {


/*!
  Returns a string with the full filepath and the resource
  appended to the end. This return value is only valid for one frame.
  \param resource file or path to be appended to the resource directory.
  \return returns a path that is valid for one frame.
*/
const char*
resource_path(const char *resource);


} // ns
} // ns


#endif // inc guard