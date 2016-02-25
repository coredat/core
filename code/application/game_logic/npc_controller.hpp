#ifndef NPC_CONTROLLER_INCLUDED_30AA5AA0_25DB_4598_9BD2_CAF55E848A17
#define NPC_CONTROLLER_INCLUDED_30AA5AA0_25DB_4598_9BD2_CAF55E848A17


#include "../common/ids_component_types.hpp"
#include "../common/ids_object_types.hpp"
#include <core/interface/component.hpp>


// TODO: Too much state in here for a controller.


class Npc_controller : public Core::Component
{
public:

  uint32_t              get_rtti() const override { return Component_type::npc_actor_controller; }
  void                  on_update(const float dt) override;
  void                  on_event(const uint32_t id, const void *data, const uint32_t size_of_data) override;

private:

  enum class State
  {
    guard,
    alerted,
  };
  
  State             m_state         = State::guard;
  float             m_sight_length  = 7.f;

}; // class


#endif // inc guard