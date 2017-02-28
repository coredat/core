#ifndef PHYS_CONFIG_INCLUDED_B8899F8C_9F8C_42A9_A6BF_FC1E079794DB
#define PHYS_CONFIG_INCLUDED_B8899F8C_9F8C_42A9_A6BF_FC1E079794DB


#include <stdint.h>


#ifndef NDEBUG
#define PHYSICS_PEDANTIC_ERROR_CHECKS
#endif


namespace Data {
namespace Physics {


extern bool g_display_debug_info;


struct Physics_config
{
  uint32_t inital_rb_count;
  uint32_t inital_trigger_count;
  float time_step;
};


inline Physics_config
physics_default()
{
  return Physics_config{
    256, // rb_count
    32, // trig_count
    (1.f / 60.f) // time step
  };
}


} // ns
} // ns


#endif // inc guard
