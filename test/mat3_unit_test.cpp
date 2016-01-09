#include <unit/unit_test.hpp>
#include <math/vec/vec.hpp>
#include <math/mat/mat.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  const float error = 0.0001f;

  // Test data
  const std::array<float, 9> zero_mat_data = {
    0,0,0,
    0,0,0,
    0,0,0,
  };

  const std::array<float, 9> one_mat_data = {
    1,1,1,
    1,1,1,
    1,1,1,
  };

  const std::array<float, 9> two_mat_data = {
    2,2,2,
    2,2,2,
    2,2,2,
  };

  const std::array<float, 9> id_mat_data = {
    1,0,0,
    0,1,0,
    0,0,1,
  };

  const std::array<float, 9> trans_mat_data = {
    1,0,0,
    0,1,0,
    0,0,1,
  };

  const std::array<float, 9> scale_mat_data = {
    1.1f,0,0,
    0,2.2f,0,
    0,0,1.f,
  };

  const std::array<float, 9> sequence_mat_data = {
    1.f,1.1f,1.2f,
    2.f,2.1f,2.2f,
    3.f,3.1f,3.2f,
  };
}


TEST(mat3_init)
{
  const math::mat3 init_c_array = math::mat3_init_with_array(&sequence_mat_data[0]);
  CHECK(mat3_equal(init_c_array, sequence_mat_data, error));

  const math::mat3 init_std_array = math::mat3_init_with_array(sequence_mat_data);
  CHECK(mat3_equal(init_std_array, sequence_mat_data, error));

  const math::mat3 init_with_no_value = math::mat3_init();
  CHECK(mat3_equal(init_with_no_value, id_mat_data, error));

  const math::mat3 init_with_value = math::mat3_init(2);
  CHECK(mat3_equal(init_with_value, two_mat_data, error));      
}


TEST(mat3_constants)
{
  const math::mat3 id_mat = math::mat3_id();
  CHECK(mat3_equal(id_mat, id_mat_data, error));

  const math::mat3 zero_mat = math::mat3_zero();
  CHECK(mat3_equal(zero_mat, zero_mat_data, error));  
}


TEST(mat3_get_values)
{
  const std::array<float, 9> mat_data = {
    1,2,3,
    5,6,7,
    9,10,11,
  };

  // Transpose
  {
    const math::mat3 original_mat = math::mat3_init_with_array(mat_data);

    const std::array<float, 9> result_data =
    {
      1,5,9,
      2,6,10,
      3,7,11,
    };

    const math::mat3 transpose = math::mat3_get_transpose(original_mat);

    CHECK(mat3_equal(transpose, result_data, error));
  }
}


TEST(mat3_multiply_mats)
{
  const std::array<float, 9> mat_data_one = {
    1,2,3,
    5,6,7,
    9,10,11,
  };
  const math::mat3 mat_one = math::mat3_init_with_array(mat_data_one);

  const std::array<float, 9> mat_data_two = {
    1,2,1,
    3,4,3,
    5,6,5,
  }; 
  const math::mat3 mat_two = math::mat3_init_with_array(mat_data_two);

  // two mats
  {
    const std::array<float, 9> expected_result = {
      22, 28, 22,
      58, 76, 58,
      94, 124, 94,
    };

    const math::mat3 result_mat = math::mat3_multiply(mat_one, mat_two);
    CHECK(mat3_equal(result_mat, expected_result, error));
  }
}


TEST(mat3_multiply_vec)
{
  const std::array<float, 9> mat_data = {
    1,2,3,
    5,6,7,
    9,10,11,
  };
  const math::mat3 mat    = math::mat3_init_with_array(mat_data);
  const math::vec3 vec    = math::vec3_init(1,2,3);
  const math::vec3 result = math::mat3_multiply(vec, mat);

  CHECK(vec3_components_are_near(result, 38, 44, 50, error))
}


TEST(mat3_scale)
{
  // Scale
  {
    const math::mat3 scale_with_floats = math::mat3_scale(1.1f, 2.2f);
    CHECK(mat3_equal(scale_with_floats, scale_mat_data, error));
  }

  // Scale
  {
    const math::vec2 scale_vec = math::vec2_init(1.1f, 2.2f);
    const math::mat3 scale_with_vec = math::mat3_scale(scale_vec);
    CHECK(mat3_equal(scale_with_vec, scale_mat_data, error));  
  }
}


TEST(mat3_translate)
{
  // Translate
  {
    const math::mat3 trans_with_floats = math::mat3_translate(10, 12);
    CHECK(mat3_equal(trans_with_floats, trans_mat_data, error));
  }

  // Translate
  {
    const math::vec2 trans_vec = math::vec2_init(10.f, 12.f);
    const math::mat3 trans_with_vec = math::mat3_translate(trans_vec);
    CHECK(mat3_equal(trans_with_vec, trans_mat_data, error));
  }
}


TEST(mat3_rotation)
{
  // Rotate
  {
    const math::mat3 rot_mat          = math::mat3_rotation_from_euler(math::pi());
    const math::vec3 point_to_rotate  = math::vec3_init(1, 0, 0);
    const math::vec3 result_vec       = math::mat3_multiply(point_to_rotate, rot_mat);

    CHECK(vec3_components_are_near(result_vec, -1, 0, 0, error));
  }
}


TEST(mat3_basic_arithmetic)
{
  const math::mat3 one_mat  = math::mat3_init_with_array(one_mat_data);

  const math::mat3 add_mat = math::mat3_add(one_mat, one_mat);
  CHECK(mat3_equal(add_mat, two_mat_data, error));

  const math::mat3 sub_mat = math::mat3_subtract(one_mat, one_mat);
  CHECK(mat3_equal(sub_mat, zero_mat_data, error));
}


TEST(mat3_determinant)
{
  const math::mat3 mat = math::mat3_init_with_array(sequence_mat_data);

  CHECK(float_is_near(mat3_get_determinant(mat), 0.f, error));
}


int main()
{
  test::run_tests();

  return 0;
}