#ifndef LOCAL_PLAYER_CONTROLLER_INCLUDED_AB2C94CC_3827_4B7B_81F1_66596B66ADD6
#define LOCAL_PLAYER_CONTROLLER_INCLUDED_AB2C94CC_3827_4B7B_81F1_66596B66ADD6


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>
#include <core/interface/entity.hpp>


class Local_player_controller final : public Core::Component
{
public:

  explicit              Local_player_controller();
                        ~Local_player_controller();

  uint32_t              get_rtti() const override { return Component_type::local_player_controller; }
  
  void                  on_start() override;
  void                  on_update(const float dt) override;
  void                  on_event(const uint32_t id, const void *data, const size_t size_of_data) override;


private:

  bool m_place_node = false;
  Core::Entity gun;

}; // class


#endif // inc guard