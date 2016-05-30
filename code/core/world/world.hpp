#ifndef WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488
#define WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488


#include <utilities/generic_id.hpp>
#include <core/world/world_fwd.hpp>
#include <core/context/context_fwd.hpp>
#include <core/world/world_setup.hpp>
#include <core/world/detail/world_detail_fwd.hpp>
#include <core/entity/entity_fwd.hpp>
#include <core/camera/camera_fwd.hpp>
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

  explicit              World(const Context &ctx,
                              const World_setup setup = World_setup{});
                        ~World();

  float                 get_delta_time() const;
  
  void                  set_delta_time_multiplier(const float multiplier);
  float                 get_delta_time_multiplier() const;

  void                  think();
  void                  get_overlapping_aabbs(const std::function<void(const Core::Collision_pair pairs[],
                                                                       const uint32_t number_of_pairs)> &callback);

  Entity_ref            find_entity_by_id(const util::generic_id id) const;
  
  void                  find_entities_by_tag(const uint32_t tag_id,
                                             Entity_ref **out_array,
                                             size_t *out_array_size);
  
private:

  friend class Core::Entity;
  friend class Core::Entity_ref;
  friend class Core::Camera;

  std::shared_ptr<const World_detail::Data>  get_world_data() const;
  std::shared_ptr<World_detail::Data>        get_world_data();

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard