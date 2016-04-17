#ifndef CONTEXT_DETAIL_INCLUDED_FDB7F20D_9B77_4AF1_892F_7B2C7361F763
#define CONTEXT_DETAIL_INCLUDED_FDB7F20D_9B77_4AF1_892F_7B2C7361F763


#include <data/core_data/core_data_fwd.hpp>


namespace Core {
namespace Context_detail {


struct Context_data
{
  Core_data::Input_pool   *input_pool = nullptr;
};


} // ns
} // ns


#endif // inc guard