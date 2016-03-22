#ifndef MUTLIPLIER_CONTROLLER_INCLUDED_B8A85AB2_E1BF_469E_AA7C_00D402F3E401
#define MUTLIPLIER_CONTROLLER_INCLUDED_B8A85AB2_E1BF_469E_AA7C_00D402F3E401


#include <core/interface/component.hpp>
#include <core/interface/entity.hpp>


class Multiplier_controller : public Core::Component
{
private:

  void            on_start() override;
  void            on_collision(const Core::Entity &actor) override;

private:

}; // class


#endif // inc guard