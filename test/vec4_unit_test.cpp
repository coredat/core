#include <unit_test/unit_test.hpp>
#include <math/vector/vector.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  const caffmath::vector4 one_vector4 = caffmath::vector4_init(1);
  const caffmath::vector4 one_two_three_four_vector4 = caffmath::vector4_init(1.1f, 2.2f, 3.3f, 4.4f);
  const caffmath::vector4 one_two_two_two_vector4 = caffmath::vector4_init(1.1f, 2.2f, 2.2f, 2.2f);
  const float error = 0.0001f;
}


TEST(vec4_constants)
{
  const caffmath::vector4 zero_vector4 = caffmath::vector4_zero();
  CHECK(vector4_components_are(zero_vector4, 0,0,0,0));

  const caffmath::vector4 one_vector4 = caffmath::vector4_one();
  CHECK(vector4_components_are(one_vector4, 1,1,1,1));

  const caffmath::vector4 zero_zero_zero_one_vector4 = caffmath::vector4_zero_zero_zero_one();
  CHECK(vector4_components_are(zero_zero_zero_one_vector4, 0,0,0,1));
}


TEST(vec4_init)
{
  const caffmath::vector4 zero_vector4 = caffmath::vector4_init(0);
  CHECK(vector4_components_are(zero_vector4, 0,0,0,0));

  const caffmath::vector4 one_vector4 = caffmath::vector4_init(1);
  CHECK(vector4_components_are(one_vector4, 1,1,1,1));
  
  const caffmath::vector4 one_two_three_four_vector4 = caffmath::vector4_init(1.1f, 2.2f, 3.3f, 4.4f);
  CHECK(vector4_components_are(one_two_three_four_vector4, 1.1f, 2.2f, 3.3f, 4.4f));

  std::array<float, 4> my_array = {1.1f, 2.2f, 3.3f, 4.4f};
  const caffmath::vector4 init_with_c_array = caffmath::vector4_init_with_array(&my_array[0]);
  CHECK(vector4_components_are(init_with_c_array, 1.1f, 2.2f, 3.3f, 4.4f));

  const caffmath::vector4 init_with_std_array = caffmath::vector4_init_with_array(my_array);
  CHECK(vector4_components_are(init_with_std_array, 1.1f, 2.2f, 3.3f, 4.4f));    
}


TEST(vec4_arithmetic)
{
  const caffmath::vector4 add_result = caffmath::vector4_add(one_two_three_four_vector4, one_vector4);
  CHECK(vector4_components_are_near(add_result, 2.1f, 3.2f, 4.3f, 5.4f, error));

  const caffmath::vector4 sub_result = caffmath::vector4_subtract(one_two_three_four_vector4, one_vector4);
  CHECK(vector4_components_are_near(sub_result, 0.1f, 1.2f, 2.3f, 3.4f, error));

  const caffmath::vector4 mul_result = caffmath::vector4_multiply(one_two_three_four_vector4, one_vector4);
  CHECK(vector4_components_are_near(mul_result, 1.1f, 2.2f, 3.3f, 4.4f, error));

  const caffmath::vector4 div_result = caffmath::vector4_divide(one_two_three_four_vector4, one_vector4);
  CHECK(vector4_components_are_near(div_result, 1.1f, 2.2f, 3.3f, 4.4f, error));
}


TEST(vec4_lerp)
{
  const caffmath::vector4 erp_start = caffmath::vector4_zero();
  const caffmath::vector4 erp_end   = caffmath::vector4_one();

  const caffmath::vector4 dt_0 = vector4_lerp(erp_start, erp_end, 0.f);
  CHECK(vector4_components_are_near(dt_0, 0.f, 0.f, 0.f, 0.f, error));

  const caffmath::vector4 dt_025 = vector4_lerp(erp_start, erp_end, 0.25f);
  CHECK(vector4_components_are_near(dt_025, 0.25f, 0.25f, 0.25f, 0.25f, error));

  const caffmath::vector4 dt_05 = vector4_lerp(erp_start, erp_end, 0.5f);
  CHECK(vector4_components_are_near(dt_05, 0.5f, 0.5f, 0.5f, 0.5f, error));

  const caffmath::vector4 dt_075 = vector4_lerp(erp_start, erp_end, 0.75f);
  CHECK(vector4_components_are_near(dt_075, 0.75f, 0.75f, 0.75f, 0.75f, error));

  const caffmath::vector4 dt_1 = vector4_lerp(erp_start, erp_end, 1.f);
  CHECK(vector4_components_are_near(dt_1, 1.f, 1.f, 1.f, 1.f, error));
}


TEST(vec4_scale)
{
  const caffmath::vector4 scale_by_2_and_half = vector4_scale(one_two_three_four_vector4, 2.5f);
  CHECK(vector4_components_are_near(scale_by_2_and_half, 2.75f, 5.5f, 8.25f, 11.0f, error));
}


TEST(vec4_length)
{
  const float length_of_vec = vector4_length(one_two_three_four_vector4);
  CHECK(float_is_near(length_of_vec, 6.02495f, error));
}


TEST(vec4_normalize)
{
  const caffmath::vector4 normalize_vector = vector4_normalize(one_two_three_four_vector4);
  CHECK(vector4_components_are_near(normalize_vector, 0.182574f, 0.365148f, 0.547723f, 0.730297f, error));
}


TEST(vec4_dot)
{
  const float dot_prod = vector4_dot(one_two_three_four_vector4, one_two_two_two_vector4);
  CHECK(float_is_near(dot_prod, 22.99f, error));
}


int main()
{
  test::run_tests();

  return 0;
}