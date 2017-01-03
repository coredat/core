#ifndef RESOURCE_ID_IMPL_INCLUDED_98C9CAF0_A112_4F1E_A73D_6F5C9655DCAE
#define RESOURCE_ID_IMPL_INCLUDED_98C9CAF0_A112_4F1E_A73D_6F5C9655DCAE


#include <stdint.h>


namespace op {


uint32_t
generate_id(const uint8_t type_id, const uint32_t instance_id);


uint8_t
type_id(const uint32_t id);


uint32_t
instance_id(const uint32_t id);


bool
resource_is_valid(const uint32_t id);


} // ns


#endif // inc guard
