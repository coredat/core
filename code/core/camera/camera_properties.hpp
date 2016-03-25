#ifndef CAMERA_PROPERTIES_INCLUDED
#define CAMERA_PROPERTIES_INCLUDED


namespace Core {


enum class Camera_type
{
  perspective,
};


namespace Camera_clear {
enum ENUM
{
  color = 1 << 0,
  depth = 1 << 1,
};
} // ns


} // ns


#endif // inc guard