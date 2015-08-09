#include <unit/unit_test.hpp>
#include <math/vec/vec.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  // Test Data
  const math::vec3 one_vec3 = math::vec3_init(1);
  const math::vec3 one_two_three_vec3 = math::vec3_init(1.1f, 2.2f, 3.3f);
  const math::vec3 one_two_two_vec3 = math::vec3_init(1.1f, 2.2f, 2.2f);

  const float error = 0.0001f;
}


TEST(vec3_constants)
{
  const math::vec3 zero_vec3 = math::vec3_zero();
  CHECK(vec3_components_are(zero_vec3, 0,0,0));

  const math::vec3 one_vec3 = math::vec3_one();
  CHECK(vec3_components_are(one_vec3, 1,1,1));

  const math::vec3 zero_zero_one_vec3 = math::vec3_zero_zero_one();
  CHECK(vec3_components_are(zero_zero_one_vec3, 0,0,1));
}


TEST(vec3_init)
{
  const math::vec3 zero_vec3 = math::vec3_init(0);
  CHECK(vec3_components_are(zero_vec3, 0,0,0));

  const math::vec3 one_vec3 = math::vec3_init(1);
  CHECK(vec3_components_are(one_vec3, 1,1,1));
  
  const math::vec3 one_two_three_vec3 = math::vec3_init(1.1f, 2.2f, 3.3f);
  CHECK(vec3_components_are(one_two_three_vec3, 1.1f, 2.2f, 3.3f));

  std::array<float, 3> my_array = {1.1f, 2.2f, 3.3f};
  const math::vec3 init_with_c_array = math::vec3_init_with_array(&my_array[0]);
  CHECK(vec3_components_are(init_with_c_array, 1.1f, 2.2f, 3.3f));

  const math::vec3 init_with_std_array = math::vec3_init_with_array(my_array);
  CHECK(vec3_components_are(init_with_std_array, 1.1f, 2.2f, 3.3f));  
}


TEST(vec3_getters)
{
  CHECK(false);
}


TEST(vec3_arithmetic)
{
    const math::vec3 add_result = math::vec3_add(one_two_three_vec3, one_vec3);
    CHECK(vec3_components_are_near(add_result, 2.1f, 3.2f, 4.3f, error));

    const math::vec3 sub_result = math::vec3_subtract(one_two_three_vec3, one_vec3);
    CHECK(vec3_components_are_near(sub_result, 0.1f, 1.2f, 2.3f, error));

    const math::vec3 mul_result = math::vec3_multiply(one_two_three_vec3, one_vec3);
    CHECK(vec3_components_are_near(mul_result, 1.1f, 2.2f, 3.3f, error));

    const math::vec3 div_result = math::vec3_divide(one_two_three_vec3, one_vec3);
    CHECK(vec3_components_are_near(div_result, 1.1f, 2.2f, 3.3f, error));
}


TEST(vec3_lerp)
{
  const math::vec3 erp_start = math::vec3_zero();
  const math::vec3 erp_end   = math::vec3_one();

  const math::vec3 dt_0 = vec3_lerp(erp_start, erp_end, 0.f);
  CHECK(vec3_components_are_near(dt_0, 0.f, 0.f, 0.f, error));

  const math::vec3 dt_025 = vec3_lerp(erp_start, erp_end, 0.25f);
  CHECK(vec3_components_are_near(dt_025, 0.25f, 0.25f, 0.25f, error));

  const math::vec3 dt_05 = vec3_lerp(erp_start, erp_end, 0.5f);
  CHECK(vec3_components_are_near(dt_05, 0.5f, 0.5f, 0.5f, error));

  const math::vec3 dt_075 = vec3_lerp(erp_start, erp_end, 0.75f);
  CHECK(vec3_components_are_near(dt_075, 0.75f, 0.75f, 0.75f, error));

  const math::vec3 dt_1 = vec3_lerp(erp_start, erp_end, 1.f);
  CHECK(vec3_components_are_near(dt_1, 1.f, 1.f, 1.f, error));
}


TEST(vec3_scale)
{
  const math::vec3 scale_by_2_and_half = vec3_scale(one_two_three_vec3, 2.5f);
  CHECK(vec3_components_are_near(scale_by_2_and_half, 2.75f, 5.5f, 8.25f, error));
}


TEST(vec3_length)
{
  const float length_of_vec = vec3_length(one_two_three_vec3);
  CHECK(float_is_near(length_of_vec, 4.11582f, error));
}


TEST(vec3_normalize)
{
  const math::vec3 normalize_vec = vec3_normalize(one_two_three_vec3);
  CHECK(vec3_components_are_near(normalize_vec, 0.2672f, 0.5345, 0.80178, error));
}


TEST(vec3_dot)
{
  const float dot_prod = vec3_dot(one_two_three_vec3, one_two_two_vec3);
  CHECK(float_is_near(dot_prod, 13.31f, error));
}


int main()
{
  test::run_tests();

  return 0;
}