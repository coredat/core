#ifndef DATA_MOTION_STATE_INCLUDED_AAED414C_E8A6_438A_84C2_843147346EF6
#define DATA_MOTION_STATE_INCLUDED_AAED414C_E8A6_438A_84C2_843147346EF6


#include <btBulletDynamicsCommon.h>
#include <core/entity_id.hpp>
#include <data/world_data/world_data_fwd.hpp>


namespace Physics {


/*!
  This connects bullet and math::transform
  so that bullet can get/set transform data.
*/
class Motion_state : public btMotionState
{
public:

  explicit      Motion_state(const Core::Entity_id e, World_data::Entity_pool *data);
                ~Motion_state();
  
  void          getWorldTransform(btTransform &world_trans) const override;
  void          setWorldTransform(const btTransform &world_trans) override;
  
private:

  const Core::Entity_id     m_entity;
  World_data::Entity_pool           *m_data;

}; // class


} // ns


#endif // inc guard