#include <unit/unit_test.hpp>
#include <math/geometry/aabb.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  const math::aabb test_aabb_01
  {
    math::vec3_init(1,1,1),
    math::vec3_init(-1,-1,-1),
    math::vec3_init(0.5, 0.5, 0.5),
    math::vec3_zero(),
  };

  const math::aabb test_aabb_02
  {
    math::vec3_init(1,1,1),
    math::vec3_init(-1,-1,-1),
    math::vec3_init(0.5, 0.5, 0.5),
    math::vec3_init(0.5, 0.5, 0.5),
  };

  const math::aabb test_aabb_03
  {
    math::vec3_init(1,1,1),
    math::vec3_init(-1,-1,-1),
    math::vec3_init(0.5, 0.5, 0.5),
    math::vec3_init(1000, 1000, 1000),
  };
}


TEST(InitFromXYZ)
{
  const uint32_t data_size(15);
  float data_xyz[] {
    1,1,1,
    0,0,0,
    2,2,2,
    4,4,4,
    -10, -10, -10,
  };

  const math::aabb result_aabb = math::aabb_from_xyz_array(data_xyz, data_size);

  const math::vec3 max_vec = math::vec3_init(4,4,4);
  const math::vec3 min_vec = math::vec3_init(-10, -10, -10);
  const math::vec3 half_extents = math::vec3_init(7,7,7);
  const math::vec3 origin = math::vec3_init(-3,-3,-3);

  CHECK(vec3_is_equal(max_vec, result_aabb.max));
  CHECK(vec3_is_equal(min_vec, result_aabb.min));
  CHECK(vec3_is_equal(half_extents, result_aabb.half_extents));
  CHECK(vec3_is_equal(origin, result_aabb.origin));
}


TEST(Extent)
{
  const math::vec3 extents = math::aabb_get_extents(test_aabb_01);
  CHECK(vec3_is_equal(math::vec3_one(), extents));
}


TEST(Intersection)
{
  CHECK(test::is_true(math::aabb_intersection_test(test_aabb_01, test_aabb_02)));
  CHECK(test::is_false(math::aabb_intersection_test(test_aabb_01, test_aabb_03)));
}


int
main()
{
  test::run_tests();

  return 0;
}
