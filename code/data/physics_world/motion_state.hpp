#ifndef DATA_MOTION_STATE_INCLUDED_AAED414C_E8A6_438A_84C2_843147346EF6
#define DATA_MOTION_STATE_INCLUDED_AAED414C_E8A6_438A_84C2_843147346EF6


#include <btBulletDynamicsCommon.h>
#include <data/entity/entity_id.hpp>


namespace Entity {

  class Data;

} // ns


namespace Physics {


class Motion_state : public btMotionState
{
public:

  explicit      Motion_state(const Entity_id e, Entity::Data *data);
  
  void          getWorldTransform(btTransform& world_trans) const override;
  void          setWorldTransform(const btTransform& world_trans) override;
  
private:

  const Entity_id m_entity;
  Entity::Data *data;

}; // class


} // ns


#endif // inc guard