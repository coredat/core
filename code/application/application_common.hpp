#ifndef APPLICATION_COMMON_INCLUDED_
#define APPLICATION_COMMON_INCLUDED_


#include <systems/environment/environment_fwd.hpp>


namespace Application {


/*!
  Common processing accross server and client machines.
*/
void common_think(
  Environment::Window *window,
  Environment::Input *input_devices);


} // ns


#endif // inc guard