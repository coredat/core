#ifndef WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488
#define WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488


#include "world_setup.hpp" // TODO: Move to fwd dec
#include <core/interface/entity.hpp>
#include <core/interface/entity_ref.hpp>
#include <stdint.h>
#include <data/core_data/core_data.hpp>
#include <data/world_data/world_data.hpp>
#include <memory>


namespace Core {


class World
{

  World(const World&) = delete;
  World& operator=(const World&) = delete;

public:

  explicit              World(const World_setup &setup);
                        ~World();

  void                  think();

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