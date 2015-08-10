#include <unit/unit_test.hpp>
#include <math/vec/vec.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  const math::vec4 one_vec4 = math::vec4_init(1);
  const math::vec4 one_two_three_four_vec4 = math::vec4_init(1.1f, 2.2f, 3.3f, 4.4f);
  const math::vec4 one_two_two_two_vec4 = math::vec4_init(1.1f, 2.2f, 2.2f, 2.2f);
  const float error = 0.0001f;
}


TEST(vec4_constants)
{
  const math::vec4 zero_vec4 = math::vec4_zero();
  CHECK(vec4_components_are(zero_vec4, 0,0,0,0));

  const math::vec4 one_vec4 = math::vec4_one();
  CHECK(vec4_components_are(one_vec4, 1,1,1,1));

  const math::vec4 zero_zero_zero_one_vec4 = math::vec4_zero_zero_zero_one();
  CHECK(vec4_components_are(zero_zero_zero_one_vec4, 0,0,0,1));
}


TEST(vec4_init)
{
  const math::vec4 zero_vec4 = math::vec4_init(0);
  CHECK(vec4_components_are(zero_vec4, 0,0,0,0));

  const math::vec4 one_vec4 = math::vec4_init(1);
  CHECK(vec4_components_are(one_vec4, 1,1,1,1));
  
  const math::vec4 one_two_three_four_vec4 = math::vec4_init(1.1f, 2.2f, 3.3f, 4.4f);
  CHECK(vec4_components_are(one_two_three_four_vec4, 1.1f, 2.2f, 3.3f, 4.4f));

  std::array<float, 4> my_array = {1.1f, 2.2f, 3.3f, 4.4f};
  const math::vec4 init_with_c_array = math::vec4_init_with_array(&my_array[0]);
  CHECK(vec4_components_are(init_with_c_array, 1.1f, 2.2f, 3.3f, 4.4f));

  const math::vec4 init_with_std_array = math::vec4_init_with_array(my_array);
  CHECK(vec4_components_are(init_with_std_array, 1.1f, 2.2f, 3.3f, 4.4f));    
}


TEST(vec4_getters)
{
  // Get x,y,z
  CHECK(vec4_get_x(one_two_three_four_vec4) == 1.1f);
  CHECK(vec4_get_y(one_two_three_four_vec4) == 2.2f);
  CHECK(vec4_get_z(one_two_three_four_vec4) == 3.3f);
  CHECK(vec4_get_w(one_two_three_four_vec4) == 4.4f);
}


TEST(vec4_get_data)
{
  std::array<float, 4> out_data;
  vec4_to_array(one_two_three_four_vec4, &out_data[0]);

  CHECK(out_data.at(0) == 1.1f && out_data.at(1) == 2.2f && out_data.at(2) == 3.3f);

  const std::array<float, 4> out_std_data = vec4_to_std_array(one_two_three_four_vec4);
  CHECK(out_data.at(0) == 1.1f && out_data.at(1) == 2.2f && out_data.at(2) == 3.3f);
}


TEST(vec4_arithmetic)
{
  const math::vec4 add_result = math::vec4_add(one_two_three_four_vec4, one_vec4);
  CHECK(vec4_components_are_near(add_result, 2.1f, 3.2f, 4.3f, 5.4f, error));

  const math::vec4 sub_result = math::vec4_subtract(one_two_three_four_vec4, one_vec4);
  CHECK(vec4_components_are_near(sub_result, 0.1f, 1.2f, 2.3f, 3.4f, error));

  const math::vec4 mul_result = math::vec4_multiply(one_two_three_four_vec4, one_vec4);
  CHECK(vec4_components_are_near(mul_result, 1.1f, 2.2f, 3.3f, 4.4f, error));

  const math::vec4 div_result = math::vec4_divide(one_two_three_four_vec4, one_vec4);
  CHECK(vec4_components_are_near(div_result, 1.1f, 2.2f, 3.3f, 4.4f, error));
}


TEST(vec4_lerp)
{
  const math::vec4 erp_start = math::vec4_zero();
  const math::vec4 erp_end   = math::vec4_one();

  const math::vec4 dt_0 = vec4_lerp(erp_start, erp_end, 0.f);
  CHECK(vec4_components_are_near(dt_0, 0.f, 0.f, 0.f, 0.f, error));

  const math::vec4 dt_025 = vec4_lerp(erp_start, erp_end, 0.25f);
  CHECK(vec4_components_are_near(dt_025, 0.25f, 0.25f, 0.25f, 0.25f, error));

  const math::vec4 dt_05 = vec4_lerp(erp_start, erp_end, 0.5f);
  CHECK(vec4_components_are_near(dt_05, 0.5f, 0.5f, 0.5f, 0.5f, error));

  const math::vec4 dt_075 = vec4_lerp(erp_start, erp_end, 0.75f);
  CHECK(vec4_components_are_near(dt_075, 0.75f, 0.75f, 0.75f, 0.75f, error));

  const math::vec4 dt_1 = vec4_lerp(erp_start, erp_end, 1.f);
  CHECK(vec4_components_are_near(dt_1, 1.f, 1.f, 1.f, 1.f, error));
}


TEST(vec4_scale)
{
  const math::vec4 scale_by_2_and_half = vec4_scale(one_two_three_four_vec4, 2.5f);
  CHECK(vec4_components_are_near(scale_by_2_and_half, 2.75f, 5.5f, 8.25f, 11.0f, error));
}


TEST(vec4_length)
{
  const float length_of_vec = vec4_length(one_two_three_four_vec4);
  CHECK(float_is_near(length_of_vec, 6.02495f, error));
}


TEST(vec4_normalize)
{
  const math::vec4 normalize_vec = vec4_normalize(one_two_three_four_vec4);
  CHECK(vec4_components_are_near(normalize_vec, 0.182574f, 0.365148f, 0.547723f, 0.730297f, error));
}


TEST(vec4_dot)
{
  const float dot_prod = vec4_dot(one_two_three_four_vec4, one_two_two_two_vec4);
  CHECK(float_is_near(dot_prod, 22.99f, error));
}


int main()
{
  test::run_tests();

  return 0;
}