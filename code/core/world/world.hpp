#ifndef WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488
#define WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488


#include <core/world/world_fwd.hpp>
#include <core/entity/entity_fwd.hpp>
#include <data/core_data/core_data.hpp> // TODO: Remove
#include <data/world_data/world_data.hpp> // TODO: remove
#include <stdint.h>
#include <memory>


namespace Core {


class World final
{

  World(const World&) = delete;
  World& operator=(const World&) = delete;

public:

  explicit              World(const World_setup &setup);
                        ~World();

  void                  think(const float dt);

  Entity                create_entity();
  Entity_ref            find_entity(const char *name);
  
//private:

  Core_data::Core       m_core_data;
  World_data::World     m_world_data;
  
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard