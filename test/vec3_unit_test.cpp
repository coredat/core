#include <unit_test/unit_test.hpp>
#include <math/vector/vector.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  // Test Data
  const caffmath::vector3 one_vector3 = caffmath::vector3_init(1);
  const caffmath::vector3 one_two_three_vector3 = caffmath::vector3_init(1.1f, 2.2f, 3.3f);
  const caffmath::vector3 one_two_two_vector3 = caffmath::vector3_init(1.1f, 2.2f, 2.2f);

  const float error = 0.0001f;
}


TEST(vec3_constants)
{
  const caffmath::vector3 zero_vector3 = caffmath::vector3_zero();
  CHECK(vector3_components_are(zero_vector3, 0,0,0));

  const caffmath::vector3 one_vector3 = caffmath::vector3_one();
  CHECK(vector3_components_are(one_vector3, 1,1,1));

  const caffmath::vector3 zero_zero_one_vector3 = caffmath::vector3_zero_zero_one();
  CHECK(vector3_components_are(zero_zero_one_vector3, 0,0,1));
}


TEST(vec3_init)
{
  const caffmath::vector3 zero_vector3 = caffmath::vector3_init(0);
  CHECK(vector3_components_are(zero_vector3, 0,0,0));

  const caffmath::vector3 one_vector3 = caffmath::vector3_init(1);
  CHECK(vector3_components_are(one_vector3, 1,1,1));
  
  const caffmath::vector3 one_two_three_vector3 = caffmath::vector3_init(1.1f, 2.2f, 3.3f);
  CHECK(vector3_components_are(one_two_three_vector3, 1.1f, 2.2f, 3.3f));

  std::array<float, 3> my_array = {1.1f, 2.2f, 3.3f};
  const caffmath::vector3 init_with_c_array = caffmath::vector3_init_with_array(&my_array[0]);
  CHECK(vector3_components_are(init_with_c_array, 1.1f, 2.2f, 3.3f));

  const caffmath::vector3 init_with_std_array = caffmath::vector3_init_with_array(my_array);
  CHECK(vector3_components_are(init_with_std_array, 1.1f, 2.2f, 3.3f));  
}


TEST(vec3_getters)
{
  CHECK(false);
}


TEST(vec3_arithmetic)
{
    const caffmath::vector3 add_result = caffmath::vector3_add(one_two_three_vector3, one_vector3);
    CHECK(vector3_components_are_near(add_result, 2.1f, 3.2f, 4.3f, error));

    const caffmath::vector3 sub_result = caffmath::vector3_subtract(one_two_three_vector3, one_vector3);
    CHECK(vector3_components_are_near(sub_result, 0.1f, 1.2f, 2.3f, error));

    const caffmath::vector3 mul_result = caffmath::vector3_multiply(one_two_three_vector3, one_vector3);
    CHECK(vector3_components_are_near(mul_result, 1.1f, 2.2f, 3.3f, error));

    const caffmath::vector3 div_result = caffmath::vector3_divide(one_two_three_vector3, one_vector3);
    CHECK(vector3_components_are_near(div_result, 1.1f, 2.2f, 3.3f, error));
}


TEST(vec3_lerp)
{
  const caffmath::vector3 erp_start = caffmath::vector3_zero();
  const caffmath::vector3 erp_end   = caffmath::vector3_one();

  const caffmath::vector3 dt_0 = vector3_lerp(erp_start, erp_end, 0.f);
  CHECK(vector3_components_are_near(dt_0, 0.f, 0.f, 0.f, error));

  const caffmath::vector3 dt_025 = vector3_lerp(erp_start, erp_end, 0.25f);
  CHECK(vector3_components_are_near(dt_025, 0.25f, 0.25f, 0.25f, error));

  const caffmath::vector3 dt_05 = vector3_lerp(erp_start, erp_end, 0.5f);
  CHECK(vector3_components_are_near(dt_05, 0.5f, 0.5f, 0.5f, error));

  const caffmath::vector3 dt_075 = vector3_lerp(erp_start, erp_end, 0.75f);
  CHECK(vector3_components_are_near(dt_075, 0.75f, 0.75f, 0.75f, error));

  const caffmath::vector3 dt_1 = vector3_lerp(erp_start, erp_end, 1.f);
  CHECK(vector3_components_are_near(dt_1, 1.f, 1.f, 1.f, error));
}


TEST(vec3_scale)
{
  const caffmath::vector3 scale_by_2_and_half = vector3_scale(one_two_three_vector3, 2.5f);
  CHECK(vector3_components_are_near(scale_by_2_and_half, 2.75f, 5.5f, 8.25f, error));
}


TEST(vec3_length)
{
  const float length_of_vec = vector3_length(one_two_three_vector3);
  CHECK(float_is_near(length_of_vec, 4.11582f, error));
}


TEST(vec3_normalize)
{
  const caffmath::vector3 normalize_vector = vector3_normalize(one_two_three_vector3);
  CHECK(vector3_components_are_near(normalize_vector, 0.2672f, 0.5345, 0.80178, error));
}


TEST(vec3_dot)
{
  const float dot_prod = vector3_dot(one_two_three_vector3, one_two_two_vector3);
  CHECK(float_is_near(dot_prod, 13.31f, error));
}


int main()
{
  test::run_tests();

  return 0;
}