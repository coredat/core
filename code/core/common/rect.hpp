#ifndef RECT_INCLUDED_46414768_F4BB_46C2_81E9_AA045615D616
#define RECT_INCLUDED_46414768_F4BB_46C2_81E9_AA045615D616


namespace Core {


/*!
  Rect represents a 2D area.
  This structure is immutable.
*/
class Rect final
{
public:

  explicit        Rect(const float top,
                       const float left,
                       const float bottom,
                       const float right);
  
  float           get_top() const     { return m_top;    }
  float           get_left() const    { return m_left;   }
  float           get_bottom() const  { return m_bottom; }
  float           get_right() const   { return m_right;  }
  
private:

  const float     m_top        = 0.f;
  const float     m_bottom     = 0.f;
  const float     m_left       = 0.f;
  const float     m_right      = 0.f;
  
}; // class


} // ns


#endif // inc guard