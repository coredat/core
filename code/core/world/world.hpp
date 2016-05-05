#ifndef WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488
#define WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488


#include <core/entity_id.hpp>
#include <core/world/world_fwd.hpp>
#include <core/world/world_setup.hpp>
#include <core/world/detail/world_detail_fwd.hpp>
#include <core/entity/entity_fwd.hpp>
#include <core/physics/physics_fwd.hpp>
#include <functional>
#include <memory>
#include <stdint.h>


namespace Core {


class World final
{

  World(const World&) = delete;
  World& operator=(const World&) = delete;

public:

  explicit              World(const World_setup setup = World_setup{});
                        ~World();

  void                  think(const float dt);
  void                  get_overlapping_aabbs(const std::function<void(const Core::Collision_pair pairs[],
                                                                       const uint32_t number_of_pairs)> &callback);

  Entity_ref            find_entity_by_id(const Core::Entity_id id) const;
  
private:

  friend class Core::Entity;
  friend class Core::Entity_ref;

  std::shared_ptr<const World_detail::World_data>  get_world_data() const;

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard