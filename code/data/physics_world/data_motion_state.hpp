#ifndef DATA_MOTION_STATE_INCLUDED_AAED414C_E8A6_438A_84C2_843147346EF6
#define DATA_MOTION_STATE_INCLUDED_AAED414C_E8A6_438A_84C2_843147346EF6


#include <btBulletDynamicsCommon.h>
#include <data/entity/entity_id.hpp>

namespace Data {

  class Entity;

} // ns


namespace Data_detail {


class Motion_state : public btMotionState
{
public:

  explicit      Motion_state(const Entity_id e, Data::Entity *data);
  
  void          getWorldTransform(btTransform& world_trans) const override;
  void          setWorldTransform(const btTransform& world_trans) override;
  
private:

  const Entity_id m_entity;
  Data::Entity *data;

}; // class


} // ns


#endif // inc guard