#ifndef COLLIDER_INCLUDED_0D024E3F_0D76_41B3_878D_6CB217255EDD
#define COLLIDER_INCLUDED_0D024E3F_0D76_41B3_878D_6CB217255EDD


#include "box_collider.hpp"


namespace Core {


class Collider
{
public:

  enum class Type
  {
    unknown,
    box,
  }; // enum

  // These ctors are implicit by design.
  constexpr
  Collider(Type type = Type::unknown,
           const float arg0 = 0.f,
           const float arg1 = 0.f,
           const float arg2 = 0.f)
  : m_type(type)
  , m_arg_01(arg0)
  , m_arg_02(arg1)
  , m_arg_03(arg2)
  {
  }
  
  
  constexpr
  Collider(Box_collider &collider)
  : Collider(Type::box,
             collider.get_x_extent(),
             collider.get_y_extent(),
             collider.get_z_extent())
  {
  }
  
  Type                  get_type() const { return m_type; };
  
  Box_collider          cast_to_box_collider() const { return Box_collider(m_arg_01, m_arg_02, m_arg_03); }
  
private:

  const Type            m_type = Type::unknown;
  const float           m_arg_01;
  const float           m_arg_02;
  const float           m_arg_03;
  
}; // class


} // ns


#endif // inc guard