#ifndef CAMERA_CONTROLLER_INCLUDED_05B921CC_58EF_4F7C_8E86_12B94D96328C
#define CAMERA_CONTROLLER_INCLUDED_05B921CC_58EF_4F7C_8E86_12B94D96328C


#include <core/interface/component.hpp>
#include <core/input/controller.hpp>
#include <core/camera/camera.hpp>
#include "../common/ids_component_types.hpp"


class Camera_controller : public Core::Component
{
private:

  uint32_t            get_rtti() const override { return Component_type::camera_controller; };
  COMPONENT_RTTI(Component_type::camera_controller)

  void                on_start() override;
  void                on_end() override;
  void                on_update(const float dt) override;

private:

  Core::Camera        m_camera;
  math::vec3          m_target_position;  

}; // class


#endif // inc guard