#ifndef COLLIDER_INCLUDED_0D024E3F_0D76_41B3_878D_6CB217255EDD
#define COLLIDER_INCLUDED_0D024E3F_0D76_41B3_878D_6CB217255EDD


namespace Core {


class Collider
{
public:

  enum class Type
  {
    box,
  }; // enum
  
  
  virtual Type          get_type() const = 0;
  
}; // class


} // ns


#endif // inc guard