#ifndef FWD_INCLUDED_BEB29E25_C993_4F70_BDBE_03E92854E313
#define FWD_INCLUDED_BEB29E25_C993_4F70_BDBE_03E92854E313


#include <stdint.h>
#include <math/transform/transform_fwd.hpp>


namespace Data {
namespace Graph {


struct Graph_data;

using transform_callback_fn = void(*)(uint32_t node, const math::transform *transform);


} // ns
} // n


#endif // inc guard