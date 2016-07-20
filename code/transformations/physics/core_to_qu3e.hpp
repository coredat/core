#ifndef CORE_TO_QU3E_INCLUDED_68E123D0_A329_4391_8A8E_272D3ED65A54
#define CORE_TO_QU3E_INCLUDED_68E123D0_A329_4391_8A8E_272D3ED65A54


#include <core/common/core_fwd.hpp>
#include <3rdparty/qu3e/q3.h>
#include <stdint.h>


namespace Physics_transform {


/*!
  Converts a Core::Rigidbody description to a qu3e one.
*/
void
convert_core_rb_to_qu3e(const Core::Rigidbody core_rb[],
                        const Core::Transform core_transform[],
                        q3Body *out_rbs[],
                        q3Scene *scene,
                        const uint32_t rb_count);


} // ns


#endif // inc guard