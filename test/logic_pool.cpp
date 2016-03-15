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
  uint32_t        data_hooks[LOGIC_POOL_NUMBER_OF_SCRIPTS];
  uint8_t         data_store[LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE * LOGIC_POOL_NUMBER_OF_SCRIPTS];
  
  
  World_data::Logic_pool pool;
  pool.entity_id = data_id;
  pool.object_store = data_store;
  pool.regd_hook = data_hooks;
  
  World_data::logic_pool_init(&pool, false);
  
  REQUIRE(pool.size == 0);
  REQUIRE(pool.capacity == LOGIC_POOL_NUMBER_OF_SCRIPTS);
  
  const Core::Entity_id id_01{1,1};
  My_test_component *comp_01 = new(World_data::logic_pool_get_slot(&pool, id_01)) My_test_component();
  REQUIRE(pool.size == 1);

  const Core::Entity_id id_02{2,2};
  My_test_component *comp_02 = new(World_data::logic_pool_get_slot(&pool, id_02)) My_test_component();
  REQUIRE(pool.size == 2);

  const Core::Entity_id id_03{3,3};
  My_test_component *comp_03 = new(World_data::logic_pool_get_slot(&pool, id_03)) My_test_component();
  REQUIRE(pool.size == 3);
  /*
    We need to add and remove components to the world.
  */
  SECTION("remove components")
  {
    World_data::logic_pool_free_slots(&pool, &id_01, 1);
    World_data::logic_pool_free_slots(&pool, &id_02, 1);
    World_data::logic_pool_free_slots(&pool, &id_03, 1);
    
    REQUIRE(pool.size == 0);
  }
  
  SECTION("On start should happen only once");
  {
    World_data::logic_pool_on_start_hook(&pool);
    World_data::logic_pool_on_start_hook(&pool);
    
    REQUIRE(comp_01->has_started == 1);
    REQUIRE(comp_02->has_started == 1);
    REQUIRE(comp_03->has_started == 1);
  }
  
  SECTION("Update hooks are called")
  {
    World_data::logic_pool_on_early_update_hook(&pool, 0.1f);
    REQUIRE(comp_01->has_early_updated == 1);
    REQUIRE(comp_02->has_early_updated == 1);
    REQUIRE(comp_02->has_early_updated == 1);
    REQUIRE(comp_01->has_updated == 0);
    REQUIRE(comp_02->has_updated == 0);
    REQUIRE(comp_02->has_updated == 0);
    
    World_data::logic_pool_on_update_hook(&pool, 0.1f);
    REQUIRE(comp_01->has_early_updated == 1);
    REQUIRE(comp_02->has_early_updated == 1);
    REQUIRE(comp_02->has_early_updated == 1);
    REQUIRE(comp_01->has_updated == 1);
    REQUIRE(comp_02->has_updated == 1);
    REQUIRE(comp_02->has_updated == 1);
  }
  
  SECTION("End hook is called")
  {
    World_data::logic_pool_free_slots(&pool, &id_01, 1);
    World_data::logic_pool_free_slots(&pool, &id_02, 1);
    World_data::logic_pool_free_slots(&pool, &id_03, 1);

    // free slots deletes the object!
//    REQUIRE(comp_01->has_ended == 1);
//    REQUIRE(comp_02->has_ended == 1);
//    REQUIRE(comp_02->has_ended == 1);
  }
  
  SECTION("Correct hooks are called - Complex")
  {
    
  }
}