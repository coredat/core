#include <unit/unit_test.hpp>
#include <math/vec/vec.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  // Test Data
  const math::vec2 one_vec2     = math::vec2_init(1.f, 1.f);
  const math::vec2 one_two_vec2 = math::vec2_init(1.1f, 2.2f);
  const math::vec2 two_two_vec2 = math::vec2_init(2.2f, 2.2f);
  const float error = 0.0001f;
}


TEST(vec2_constants)
{
  const math::vec2 zero_vec2 = math::vec2_zero();
  CHECK(vec2_components_are(zero_vec2, 0,0));

  const math::vec2 one_vec2 = math::vec2_one();
  CHECK(vec2_components_are(one_vec2, 1,1));

  const math::vec2 zero_one_vec2 = math::vec2_zero_one();
  CHECK(vec2_components_are(zero_one_vec2, 0,1));
}


TEST(vec2_init)
{
  const math::vec2 zero_vec2 = math::vec2_init(0);
  CHECK(vec2_components_are(zero_vec2, 0,0));

  const math::vec2 one_vec2 = math::vec2_init(1);
  CHECK(vec2_components_are(one_vec2, 1,1));
  
  const math::vec2 one_two_vec2 = math::vec2_init(1.1f, 2.2f);
  CHECK(vec2_components_are(one_two_vec2, 1.1f, 2.2f));

  std::array<float, 2> my_array = {1.1f, 2.2f};
  const math::vec2 init_with_c_array = math::vec2_init_with_array(&my_array[0]);
  CHECK(vec2_components_are(init_with_c_array, 1.1f, 2.2f));

  const math::vec2 init_with_std_array = math::vec2_init_with_array(my_array);
  CHECK(vec2_components_are(init_with_std_array, 1.1f, 2.2f));
}


TEST(vec2_get_components)
{
  CHECK(test::is_equal<float>(1.1f, math::vec2_get_x(one_two_vec2)));
  CHECK(test::is_equal<float>(2.2f, math::vec2_get_y(one_two_vec2)));
}


TEST(vec2_to_array)
{
  auto std_arr = math::vec2_to_std_array(one_two_vec2);

  CHECK(test::is_equal<float>(1.1f, std_arr.at(0)));
  CHECK(test::is_equal<float>(2.2f, std_arr.at(1)));

  float c_arr[2];
  math::vec2_to_array(one_two_vec2, c_arr);

  CHECK(test::is_equal<float>(1.1f, c_arr[0]));
  CHECK(test::is_equal<float>(2.2f, c_arr[1]));
}


TEST(vec2_arithmetic)
{
  const math::vec2 add_result = math::vec2_add(one_two_vec2, one_vec2);
  CHECK(vec2_components_are_near(add_result, 2.1f, 3.2f, error));

  const math::vec2 sub_result = math::vec2_subtract(one_two_vec2, one_vec2);
  CHECK(vec2_components_are_near(sub_result, 0.1f, 1.2f, error));

  const math::vec2 mul_result = math::vec2_multiply(one_two_vec2, one_vec2);
  CHECK(vec2_components_are_near(mul_result, 1.1f, 2.2f, error));

  const math::vec2 div_result = math::vec2_divide(one_two_vec2, one_vec2);
  CHECK(vec2_components_are_near(div_result, 1.1f, 2.2f, error));
}


TEST(vec2_lerp)
{
  const math::vec2 erp_start = math::vec2_zero();
  const math::vec2 erp_end   = math::vec2_one();

  const math::vec2 dt_0 = vec2_lerp(erp_start, erp_end, 0.f);
  CHECK(vec2_components_are_near(dt_0, 0.f, 0.f, error));

  const math::vec2 dt_025 = vec2_lerp(erp_start, erp_end, 0.25f);
  CHECK(vec2_components_are_near(dt_025, 0.25f, 0.25f, error));

  const math::vec2 dt_05 = vec2_lerp(erp_start, erp_end, 0.5f);
  CHECK(vec2_components_are_near(dt_05, 0.5f, 0.5f, error));

  const math::vec2 dt_075 = vec2_lerp(erp_start, erp_end, 0.75f);
  CHECK(vec2_components_are_near(dt_075, 0.75f, 0.75f, error));

  const math::vec2 dt_1 = vec2_lerp(erp_start, erp_end, 1.f);
  CHECK(vec2_components_are_near(dt_1, 1.f, 1.f, error));
}


TEST(vec2_scale)
{
  const math::vec2 scale_by_2_and_half = vec2_scale(one_two_vec2, 2.5f);
  CHECK(vec2_components_are_near(scale_by_2_and_half, 2.75f, 5.5f, error));
}


TEST(vec2_normal)
{
  const math::vec2 normalize_vec = vec2_normalize(one_two_vec2);
  CHECK(vec2_components_are_near(normalize_vec, 0.4472f, 0.89442f, error));
}


TEST(vec2_dot)
{
  const float dot_prod = vec2_dot(one_two_vec2, two_two_vec2);
  CHECK(float_is_near(dot_prod, 7.26f, error));
}


TEST(vec2_length)
{
  const float length_of_vec = vec2_length(one_two_vec2);
  CHECK(float_is_near(length_of_vec, 2.4596f, error));
}


int main()
{
  test::run_tests();

  return 0;
}