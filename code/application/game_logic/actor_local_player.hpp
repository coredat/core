#ifndef ACTOR_PLAYER_INCLUDED_BF0E0298_8B5D_4307_985A_5CDD26FD16BF
#define ACTOR_PLAYER_INCLUDED_BF0E0298_8B5D_4307_985A_5CDD26FD16BF


#include "actor_base.hpp"
#include <application/common/ids_component_types.hpp>
#include <core/interface/component.hpp>
#include <math/math.hpp>


class Actor_local_player : public Actor_base
{
public:

  explicit Actor_local_player();
  
  uint32_t get_rtti() const override { return Component_type::actor_local; }

  /** Hooks **/
  void on_start() override;
  void on_update(const float dt) override;

  /** Actions **/
  void action();
  
private:
  
  Core::Entity_id cube_id;
  bool m_place_node;

};



#endif // inc guard