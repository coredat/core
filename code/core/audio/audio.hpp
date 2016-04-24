#ifndef AUDIO_INCLUDED_2245F334_30F4_4507_9BA8_070C29036E20
#define AUDIO_INCLUDED_2245F334_30F4_4507_9BA8_070C29036E20


#include <core/context/context_fwd.hpp>
#include <math/vec/vec_types.hpp>
#include <stdint.h>


namespace Core {
namespace Audio {


void
ear_set(Core::Context &context,
        const math::vec3 set_position);


} // ns
} // ns


#endif // inc guard