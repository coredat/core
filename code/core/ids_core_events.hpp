#ifndef IDS_EVENTS_INCLUDED_13B779A3_0D33_41A2_A513_5357D7C4C1C8
#define IDS_EVENTS_INCLUDED_13B779A3_0D33_41A2_A513_5357D7C4C1C8


#include <stdint.h>


namespace Core {
namespace Event_id {
enum ENUM : uint32_t {

  application_start = 0,
  application_end,
  
  input_button_down,
  input_button_up,


  end_of_core_events // nothing below here,

}; // enum
} // ns
} // ns


#endif // inc guard