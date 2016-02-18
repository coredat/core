#ifndef NETWORK_PLAYER_CONTROLLER_INCLUDED_
#define NETWORK_PLAYER_CONTROLLER_INCLUDED_


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>


class Network_player_controller final : public Core::Component
{
public:

  explicit              Network_player_controller();
                        ~Network_player_controller();
  
  uint32_t              get_rtti() const override { return Component_type::network_player_controller; }
  
  void                  on_start() override;
  void                  on_update(const float dt) override;
  void                  on_event(const uint32_t id, const void *data, const size_t size_of_data) override;


}; // class


#endif // inc guard