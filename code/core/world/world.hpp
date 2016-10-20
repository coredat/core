#ifndef WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488
#define WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <data/fwd_dec_data.hpp>
#include <data/world_data.hpp>
#include <core/world/world_setup.hpp>


namespace Core {


enum class Collision_type
{
  enter,
  exit,
};


typedef void (*Collision_callback)(const Collision_type type, const Core::Collision_pair &collision);


/*!
  World
  --
  The world is a collection of entities in the world.
*/
class World final
{

  World(const World&) = delete;
  World& operator=(const World&) = delete;

public:

  explicit              World(Context &ctx,
                              const World_setup setup = World_setup{});
  
                        ~World();
  
                        World(World &&other);
  World&                operator=(World &&other);

  float                 get_delta_time() const;
  void                  set_delta_time_multiplier(const float multiplier);
  float                 get_delta_time_multiplier() const;
  uint32_t              get_time_running() const;

  void                  think();
  
  size_t                get_entity_count_in_world() const;
  
  void                  set_collision_callback(Collision_callback callback);

  Entity_ref            find_entity_by_id(const util::generic_id id) const;
  Collision             find_entity_by_ray(const Ray ray) const;
  Entity_ref            find_entity_by_name(const char *name) const;

  void                  find_entities_by_tag(const uint32_t tag_id,
                                             Entity_ref **out_array,
                                             size_t *out_array_size);

  void                  find_entities_by_name(const char *name,
                                              Entity_ref **out_array,
                                              size_t *out_array_size);
  
private:

  friend class Core::Entity_interface;
  friend class Core::Camera;
  friend class Core::Ray;

  uint32_t                           get_id() const;
  std::shared_ptr<const Data::World> get_world_data() const;
  std::shared_ptr<Data::World>       get_world_data();

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard