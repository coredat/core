#ifndef STATE_INCLUDED_94905108_2872_4B1E_B37D_76F55FFE1673
#define STATE_INCLUDED_94905108_2872_4B1E_B37D_76F55FFE1673


#include <stdint.h>


// -- Status -- //

constexpr uint8_t opStatus_PENDING  = 0;
constexpr uint8_t opStatus_VALID    = 1;
constexpr uint8_t opStatus_INVALID  = 2;

// -- Boolean -- //

constexpr uint8_t opBool_FALSE = 0;
constexpr uint8_t opBool_TRUE  = 1;

// -- Pixel Formats -- //

constexpr uint8_t opPixelFormat_UNKNOWN  = 0;
constexpr uint8_t opPixelFormat_R8       = 1;
constexpr uint8_t opPixelFormat_RG8      = 2;
constexpr uint8_t opPixelFormat_RGB8     = 3;
constexpr uint8_t opPixelFormat_RGBA8    = 4;
constexpr uint8_t opPixelFormat_R32F     = 5;
constexpr uint8_t opPixelFormat_RG32F    = 6;
constexpr uint8_t opPixelFormat_RGB32F   = 7;
constexpr uint8_t opPixelFormat_RGBA32F  = 8;

// -- Dimentions -- //

constexpr uint8_t opDimention_UNKNOWN = 0;
constexpr uint8_t opDimention_ONE     = 1;
constexpr uint8_t opDimention_TWO     = 2;
constexpr uint8_t opDimention_THREE   = 3;

// -- CPU Access -- //

constexpr uint8_t opAccess_STATIC   = 0;
constexpr uint8_t opAccess_DYNAMIC  = 1;

// -- Primitive Types -- //

const uint8_t opPrimitive_TRIANGLE       = 0;
const uint8_t opPrimitive_POINT          = 1;
const uint8_t opPrimitive_LINE           = 2;
const uint8_t opPrimitive_LINE_STRIP     = 3;
const uint8_t opPrimitive_TRIANGLE_STRIP = 4;
const uint8_t opPrimitive_TRIANGLE_FAN   = 5;
const uint8_t opPrimitive_QUAD           = 6;

// -- Cull -- //

constexpr uint8_t opCullFace_BACK  = 0;
constexpr uint8_t opCullFace_FRONT = 1;
constexpr uint8_t opCullFace_NONE  = 2;
constexpr uint8_t opCullFace_BOTH  = 3;

// -- Winding Order -- //

constexpr uint8_t opWindingOrder_CCW = 0;
constexpr uint8_t opWindingOrder_CW  = 1;

// -- Types -- //

constexpr uint8_t opType_NONE       = 0;
constexpr uint8_t opType_FLOAT      = 1;
constexpr uint8_t opType_FLOAT2     = 2;
constexpr uint8_t opType_FLOAT3     = 3;
constexpr uint8_t opType_FLOAT4     = 4;
constexpr uint8_t opType_INT        = 5;
constexpr uint8_t opType_INT2       = 6;
constexpr uint8_t opType_INT3       = 7;
constexpr uint8_t opType_INT4       = 8;
constexpr uint8_t opType_MAT2       = 9;
constexpr uint8_t opType_MAT3       = 10;
constexpr uint8_t opType_MAT4       = 11;
constexpr uint8_t opType_TEXTURE_1D = 12;
constexpr uint8_t opType_TEXTURE_2D = 13;
constexpr uint8_t opType_TEXTURE_3D = 14;

// -- Wrap Mode -- //

constexpr uint8_t opWrapMode_CLAMP  = 0;
constexpr uint8_t opWrapMode_REPEAT = 1;

// -- Filter Mode -- //

constexpr uint8_t opFilterMode_POINT       = 0;
constexpr uint8_t opFilterMode_BILINEAR    = 1;
constexpr uint8_t opFilterMode_TRILINEAR   = 2;
constexpr uint8_t opFilterMode_ANISOTROPIC = 3;

// -- Support Status -- //

constexpr uint8_t opSupport_NONE    = 0;
constexpr uint8_t opSupport_NATIVE  = 1;
constexpr uint8_t opSupport_PARTIAL = 2;

#endif // inc guard
