#ifndef ENTITY_CALLBACK_INCLUDED_9A117C9A_BBBC_4218_BDA1_85DE29447E77
#define ENTITY_CALLBACK_INCLUDED_9A117C9A_BBBC_4218_BDA1_85DE29447E77


#include <data/fwd_dec_data.hpp>
#include <stddef.h>
#include <stdint.h>


namespace Entity_detail {


void
set_entity_collision_callback(const uint32_t this_id,
                              Data::Entity_data *entity_data,
                              const uintptr_t callback_ptr,
                              const uintptr_t user_data);


} // ns


#endif // inc guard
