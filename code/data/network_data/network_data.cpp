#include "network_data.hpp"


namespace
{

  Net_data::Network_data *net_data = nullptr;

}



namespace Net_data {


void
set_net_data(Network_data *set)
{
  net_data = set;
}


Network_data*
get_net_data()
{
  return net_data;
}


} // ns