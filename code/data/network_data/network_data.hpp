#ifndef NETWORK_DATA_INCLUDED_
#define NETWORK_DATA_INCLUDED_


#include "interpolation_pool.hpp"
#include "net_entity_pool.hpp"


namespace Net_data {


struct Network_data
{
  Net_entity_pool     *net_entity_pool;
  Interpolation_pool  *interpolation_pool;
};


void
set_net_data(Network_data *net_data);


Network_data*
get_net_data();


} // ns


#endif // inc guard