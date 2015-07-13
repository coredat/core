#include <unit_test/unit_test.hpp>
#include <math/vector/vector.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  // Teest Data
  const caffmath::vector2 one_two_vector2 = caffmath::vector2_init(1.1f, 2.2f);
  const caffmath::vector2 two_two_vector2 = caffmath::vector2_init(2.2f, 2.2f);
  const float error = 0.0001f;
}


TEST(vec2_constants)
{
  const caffmath::vector2 zero_vector2 = caffmath::vector2_zero();
  CHECK(vector2_components_are(zero_vector2, 0,0));

  const caffmath::vector2 one_vector2 = caffmath::vector2_one();
  CHECK(vector2_components_are(one_vector2, 1,1));

  const caffmath::vector2 zero_one_vector2 = caffmath::vector2_zero_one();
  CHECK(vector2_components_are(zero_one_vector2, 0,1));
}


TEST(vec2_init)
{
  const caffmath::vector2 zero_vector2 = caffmath::vector2_init(0);
  CHECK(vector2_components_are(zero_vector2, 0,0));

  const caffmath::vector2 one_vector2 = caffmath::vector2_init(1);
  CHECK(vector2_components_are(one_vector2, 1,1));
  
  const caffmath::vector2 one_two_vector2 = caffmath::vector2_init(1.1f, 2.2f);
  CHECK(vector2_components_are(one_two_vector2, 1.1f, 2.2f));

  std::array<float, 2> my_array = {1.1f, 2.2f};
  const caffmath::vector2 init_with_c_array = caffmath::vector2_init_with_array(&my_array[0]);
  CHECK(vector2_components_are(init_with_c_array, 1.1f, 2.2f));

  const caffmath::vector2 init_with_std_array = caffmath::vector2_init_with_array(my_array);
  CHECK(vector2_components_are(init_with_std_array, 1.1f, 2.2f));
}


TEST(vec2_arithmetic)
{
  const caffmath::vector2 add_result = caffmath::vector2_add(one_two_vector2, one_vector2);
  CHECK(vector2_components_are_near(add_result, 2.1f, 3.2f, error));

  const caffmath::vector2 sub_result = caffmath::vector2_subtract(one_two_vector2, one_vector2);
  CHECK(vector2_components_are_near(sub_result, 0.1f, 1.2f, error));

  const caffmath::vector2 mul_result = caffmath::vector2_multiply(one_two_vector2, one_vector2);
  CHECK(vector2_components_are_near(mul_result, 1.1f, 2.2f, error));

  const caffmath::vector2 div_result = caffmath::vector2_divide(one_two_vector2, one_vector2);
  CHECK(vector2_components_are_near(div_result, 1.1f, 2.2f, error));
}


TEST(vec2_lerp)
{
  const caffmath::vector2 erp_start   = caffmath::vector2_zero();
  const caffmath::vector2 erp_end   = caffmath::vector2_one();

  const caffmath::vector2 dt_0 = vector2_lerp(erp_start, erp_end, 0.f);
  CHECK(vector2_components_are_near(dt_0, 0.f, 0.f, error));

  const caffmath::vector2 dt_025 = vector2_lerp(erp_start, erp_end, 0.25f);
  CHECK(vector2_components_are_near(dt_025, 0.25f, 0.25f, error));

  const caffmath::vector2 dt_05 = vector2_lerp(erp_start, erp_end, 0.5f);
  CHECK(vector2_components_are_near(dt_05, 0.5f, 0.5f, error));

  const caffmath::vector2 dt_075 = vector2_lerp(erp_start, erp_end, 0.75f);
  CHECK(vector2_components_are_near(dt_075, 0.75f, 0.75f, error));

  const caffmath::vector2 dt_1 = vector2_lerp(erp_start, erp_end, 1.f);
  CHECK(vector2_components_are_near(dt_1, 1.f, 1.f, error));
}


TEST(vec2_scale)
{
  const caffmath::vector2 scale_by_2_and_half = vector2_scale(one_two_vector2, 2.5f);
  CHECK(vector2_components_are_near(scale_by_2_and_half, 2.75f, 5.5f, error));
}


TEST(vec2_normal)
{
  const caffmath::vector2 normalize_vector = vector2_normalize(one_two_vector2);
  CHECK(vector2_components_are_near(normalize_vector, 0.4472f, 0.89442f, error));
}


TEST(vec2_dot)
{
  const float dot_prod = vector2_dot(one_two_vector2, two_two_vector2);
  CHECK(float_is_near(dot_prod, 7.26f, error));
}


TEST(vec2_length)
{
  const float length_of_vec = vector2_length(one_two_vector2);
  CHECK(float_is_near(length_of_vec, 2.4596f, error));
}


int main()
{
  test::run_tests();

  return 0;
}