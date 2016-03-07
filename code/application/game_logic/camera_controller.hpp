#ifndef CAMERA_CONTROLLER_INCLUDED_
#define CAMERA_CONTROLLER_INCLUDED_


#include <core/interface/component.hpp>
#include <core/input/controller.hpp>
#include <core/camera/camera.hpp>


class Camera_controller : public Core::Component
{
private:

  uint32_t            get_rtti() const override { return 1; };
  COMPONENT_RTTI(2)

  void                on_start() override;
  void                on_end() override;
  void                on_update(const float dt) override;

private:

  Core::Camera        m_camera;

}; // class


#endif // inc guard