#ifndef EVENT_MANAGER_INCLUDED_26E97645_88B6_47D2_B040_E2FDFD3B6120
#define EVENT_MANAGER_INCLUDED_26E97645_88B6_47D2_B040_E2FDFD3B6120


#include <stdint.h>
#include <stddef.h>
#include <functional>


/*!
  The main goal of this is to provide.
  systems -> components comms.
  components -> components comms.
  
  Its not really intended for inter system comms.
  although that should work, it just creates a bad
  flow.
*/
namespace Core {
namespace Event_manager {


void
initialize();


void
de_initialize();


void*
get_memory_chunk(const size_t size_of_data);


void
send_message(const uint32_t event_id, const uint8_t *data, const size_t size_of_data);


size_t
listen_for_message(const uint32_t event_id, const &std::functional<void(uint32_t, uint8_t*, size_t)> &callback);


void
stop_listening_for_message(const uint32_t event_id, const size_t listener_id);


} // ns
} // ns


#endif // inc guard