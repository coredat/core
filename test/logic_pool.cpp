#include "test_framework/catch.hpp"
#include <data/world_data/logic_pool.hpp>
#include <stdatomic.h>

namespace
{
  _Atomic uint32_t counter(0);
//  uint32_t counter(0);

  class My_test_component : public Core::Component
  {
  public:
  
    My_test_component()
    : Core::Component()
    , id(++counter)
    {
    }
  
    void
    on_start() override
    {
      ++has_started;
    }
    
    void
    on_early_update(const float dt) override
    {
      ++has_early_updated;
    }
    
    void
    on_update(const float dt) override
    {
      ++has_updated;
    }
    
    void
    on_end() override
    {
      ++has_ended;
    }
  
    uint32_t has_started        = 0;
    uint32_t has_early_updated  = 0;
    uint32_t has_updated        = 0;
    uint32_t has_ended          = 0;
    const uint32_t id;
  
  };
}


TEST_CASE("LogicPool")
{
  // Storeage
  Core::Entity_id data_id[LOGIC_POOL_NUMBER_OF_SCRIPTS];
  void            *data_loc[LOGIC_POOL_NUMBER_OF_SCRIPTS];
  void            *data_obj_in_use[LOGIC_POOL_NUMBER_OF_SCRIPTS];
  void            *data_pending_start[LOGIC_POOL_NUMBER_OF_SCRIPTS];
  uint8_t         data_store[LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE * LOGIC_POOL_NUMBER_OF_SCRIPTS];
  
  
  World_data::Logic_pool pool;
  pool.entity_id = data_id;
  pool.object_locations = data_loc;
  pool.objects_in_use = data_obj_in_use;
  pool.objects_on_start_pending_hooks = data_pending_start;
  pool.storage = data_store;
  
  World_data::logic_pool_init(&pool, false);
  
  REQUIRE(pool.objects_in_use_size == 0);
  REQUIRE(pool.size == LOGIC_POOL_NUMBER_OF_SCRIPTS);
  REQUIRE(pool.storage_size == LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE);
  REQUIRE(pool.objects_on_start_pending_hooks_size == 0);
  
  /*
    We need to add and remove components to the world.
  */
  SECTION("Add and remove components")
  {
    My_test_component *comp_01 = new(World_data::logic_pool_get_slot(&pool, Core::Entity_id{1,1})) My_test_component();
  
    REQUIRE(pool.objects_in_use_size == 1);
    REQUIRE(pool.objects_on_start_pending_hooks_size == 1);
    
    My_test_component *comp_02 = new(World_data::logic_pool_get_slot(&pool, Core::Entity_id{2,2})) My_test_component();
  
    REQUIRE(pool.objects_in_use_size == 2);
    REQUIRE(pool.objects_on_start_pending_hooks_size == 2);
    
    My_test_component *comp_03 = new(World_data::logic_pool_get_slot(&pool, Core::Entity_id{3,3})) My_test_component();
  
    REQUIRE(pool.objects_in_use_size == 3);
    REQUIRE(pool.objects_on_start_pending_hooks_size == 3);
    
    World_data::logic_pool_free_slots(&pool, Core::Entity_id{1,1});
    World_data::logic_pool_free_slots(&pool, Core::Entity_id{2,2});
    World_data::logic_pool_free_slots(&pool, Core::Entity_id{3,3});
    
    REQUIRE(pool.objects_in_use_size == 0);
    REQUIRE(pool.objects_on_start_pending_hooks_size == 0);
  }
  
  SECTION("Correct hooks are called - Simple")
  {
  }
  
  SECTION("Correct hooks are called - Complex")
  {
  }
}