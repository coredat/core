#ifndef APPLICATION_COMMON_INCLUDED_90D3AF40_A84F_4047_B7CD_2B58FE69468A
#define APPLICATION_COMMON_INCLUDED_90D3AF40_A84F_4047_B7CD_2B58FE69468A


#include <data/core_data/core_data_fwd.hpp>
#include <systems/environment/environment_fwd.hpp>


namespace Application {


/*!
  Common processing accross server and client machines.
*/
void common_think(
  Environment::Window *window,
  Environment::Input *input_devices,
  Core_data::Core *core_data);


} // ns


#endif // inc guard