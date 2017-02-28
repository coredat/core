#ifndef MOTION_STATE_INCLUDED_2AA60ABD_8B4A_4050_9D9E_8D09DAD31DB4
#define MOTION_STATE_INCLUDED_2AA60ABD_8B4A_4050_9D9E_8D09DAD31DB4


#include <btBulletDynamicsCommon.h>


namespace Bullet_detail {


ATTRIBUTE_ALIGNED16(struct) Core_motion_state : public btDefaultMotionState
{
	Core_motion_state(const uintptr_t user_data,
                    const btTransform& startTrans = btTransform::getIdentity(),
                    const btTransform& centerOfMassOffset = btTransform::getIdentity());

	///synchronizes world transform from user to physics
	void
  getWorldTransform(btTransform& centerOfMassWorldTrans ) const override;

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	void
  setWorldTransform(const btTransform& centerOfMassWorldTrans) override;
};


} // ns


#endif // inc guard
