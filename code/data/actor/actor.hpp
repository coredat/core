#ifndef ACTOR_INCLUDED_
#define ACTOR_INCLUDED_


namespace Actor {


class Actor_base
{
public:


  explicit Actor_base()
  {
  }
  
  
  void
  move_forward(const float fwd)
  {
  
  }
  
  
  void
  turn(const float trn)
  {
    accum_radians_turn += trn;
  }
  
  
  void
  strafe(const float st)
  {
    
  }
  
  
  void
  think(const float dt)
  {
  }
  
  
private:

  float accum_fwd = 0.f;
  float accum_strafe = 0.f;
  float accum_radians_turn = 0.f;


}; // class


} // ns


#endif // inc guard