#ifndef TRANSFORM_INCLUDED_047C029A_0690_4259_8C7F_EC5C637776A4
#define TRANSFORM_INCLUDED_047C029A_0690_4259_8C7F_EC5C637776A4


#include <math/math.hpp>


namespace Core {


/*!
  Transform represents an Entities place in the world.
*/
class Transform final
{
public:

  explicit            Transform();
  explicit            Transform(const math::vec3 pos,
                                const math::vec3 scale,
                                const math::quat rot);
  
  math::vec3          get_position() const                { return m_position; }
  void                set_position(const math::vec3 pos)  { m_position = pos;  }
  
  math::vec3          get_eular() const;
  math::quat          get_rotation() const                { return m_rotation; }
  void                set_rotation(const math::quat rot)  { m_rotation = rot;  }
  
  math::vec3          get_scale() const                   { return m_scale;    }
  void                set_scale(const math::vec3 scale)   { m_scale = scale;   }
  
  math::vec3          get_forward() const;
  math::vec3          get_up() const;
  math::vec3          get_left() const;
  
private:

  math::vec3          m_position  = math::vec3_zero();
  math::vec3          m_scale     = math::vec3_one();
  math::quat          m_rotation  = math::quat_init();

}; // class


} // ns


#endif // inc guard