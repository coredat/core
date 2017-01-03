#ifndef COMMON_INCLUDED_8F3B64C7_9A17_4C60_B9EC_F91E79A9EA1B
#define COMMON_INCLUDED_8F3B64C7_9A17_4C60_B9EC_F91E79A9EA1B


#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "../../include/op/fwd.hpp"


#if defined(OP_BUFFER_API_TEST)

#include "../api/test/test_api.hpp"
#define API op::test_api

#elif defined(OP_BUFFER_API_OGL)

#include "../api/ogl/ogl_api.hpp"
#define API op::ogl

#else

#define API op::unsupported

#endif


#endif // inc guard
