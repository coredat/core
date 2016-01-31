#ifndef IDS_COMPONENT_TYPES_INCLUDED_
#define IDS_COMPONENT_TYPES_INCLUDED_


#include <stdint.h>


namespace Component_type {
enum ENUM : uint32_t {

  invalid,    // zero 

  actor_local,
  actor_network,

}; // enum
} // ns


#endif // inc guard