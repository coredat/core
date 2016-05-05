#ifndef COLLIDER_INCLUDED_0D024E3F_0D76_41B3_878D_6CB217255EDD
#define COLLIDER_INCLUDED_0D024E3F_0D76_41B3_878D_6CB217255EDD


namespace Core {


class Collider final
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
           const float arg1 = 0.f,
           const float arg2 = 0.f,
           const float arg3 = 0.f)
  : m_type(type)
  , m_arg_01(arg1)
  , m_arg_02(arg2)
  , m_arg_03(arg3)
  {
  }
  
  Type                  get_type() const    { return m_type;   }
  float                 get_arg_01() const  { return m_arg_01; }
  float                 get_arg_02() const  { return m_arg_02; }
  float                 get_arg_03() const  { return m_arg_03; }
  
private:

  const Type            m_type = Type::unknown;
  const float           m_arg_01;
  const float           m_arg_02;
  const float           m_arg_03;
  
}; // class


} // ns


#endif // inc guard