#include <unit/unit_test.hpp>
#include <math/vec/vec.hpp>
#include <math/general/to_string.hpp>
#include <test_asserts.hpp>


namespace
{
  const float error = 0.0001f;

  // Test data
  const std::array<float, 16> zero_mat_data = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
  };

  const std::array<float, 16> one_mat_data = {
    1,1,1,1,
    1,1,1,1,
    1,1,1,1,
    1,1,1,1,
  };

  const std::array<float, 16> two_mat_data = {
    2,2,2,2,
    2,2,2,2,
    2,2,2,2,
    2,2,2,2,
  };

  const std::array<float, 16> four_mat_data = {
    4,4,4,4,
    4,4,4,4,
    4,4,4,4,
    4,4,4,4,
  };

  const std::array<float, 16> sixteen_mat_data = {
    16,16,16,16,
    16,16,16,16,
    16,16,16,16,
    16,16,16,16,
  };  

  const std::array<float, 16> id_mat_data = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,
  };

  const std::array<float, 16> trans_mat_data = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    10,12,14,1,
  };

  const std::array<float, 16> scale_mat_data = {
    1.1f,0,0,0,
    0,2.2f,0,0,
    0,0,3.3f,0,
    0,0,0,1,
  };

  const std::array<float, 16> sequence_mat_data = {
    1.f,1.1f,1.2f,1.3f,
    2.f,2.1f,2.2f,2.3f,
    3.f,3.1f,3.2f,3.3f,
    4.f,4.1f,4.2f,4.3f,
  };
}


TEST(mat4_init)
{
  const math::mat4 init_c_array = math::mat4_init_with_array(&sequence_mat_data[0]);
  CHECK(mat4_equal(init_c_array, sequence_mat_data, error));

  const math::mat4 init_std_array = math::mat4_init_with_array(sequence_mat_data);
  CHECK(mat4_equal(init_std_array, sequence_mat_data, error));

  const math::mat4 init_with_no_value = math::mat4_init();
  CHECK(mat4_equal(init_with_no_value, id_mat_data, error));

  const math::mat4 init_with_value = math::mat4_init(2);
  CHECK(mat4_equal(init_with_value, two_mat_data, error));      
}


TEST(mat4_constants)
{
  const math::mat4 id_mat = math::mat4_id();
  CHECK(mat4_equal(id_mat, id_mat_data, error));

  const math::mat4 zero_mat = math::mat4_zero();
  CHECK(mat4_equal(zero_mat, zero_mat_data, error));  
}


TEST(mat4_get_values)
{
  const std::array<float, 16> mat_data = {
    1,2,3,4,
    5,6,7,8,
    9,10,11,12,
    13,14,15,16,
  };

  // Sub mat
  {
    const std::array<float, 9> mat_result_data = {
      1,2,3,
      5,6,7,
      9,10,11
    };

    const math::mat4 mat = math::mat4_init_with_array(mat_data);
    const math::mat3 sub_mat = math::mat4_get_sub_mat3(mat);

    CHECK(mat3_equal(sub_mat, mat_result_data, error));
  }

  // Transpose
  {
    const math::mat4 original_mat = math::mat4_init_with_array(mat_data);

    const std::array<float, 16> result_data =
    {
      1,5,9,13,
      2,6,10,14,
      3,7,11,15,
      4,8,12,16,
    };

    const math::mat4 transpose = math::mat4_get_transpose(original_mat);

    CHECK(mat4_equal(transpose, result_data, error));
  }
}


TEST(mat4_camera)
{
  // Look at
  {
    const math::vec3 position = math::vec3_zero();
    const math::vec3 target   = math::vec3_init(1,1,1);
    const math::vec3 up       = math::vec3_init(0,1,0);

    const math::mat4 look_at_mat = math::mat4_lookat(position, target, up);
    CHECK(false);
  }
  
  // Projection
  {
    const float width         = 800.f;
    const float height        = 600.f;
    const float near_plane    = 1.f;
    const float far_plane     = 100.f;
    const float field_of_view = math::quart_tau();

    const math::mat4 proj_mat = math::mat4_projection(width, height, near_plane, far_plane, field_of_view);
    CHECK(false);
  }
}


TEST(mat4_scale)
{
  // Scale
  {
    const math::mat4 scale_with_floats = math::mat4_scale(1.1f, 2.2f, 3.3f);
    CHECK(mat4_equal(scale_with_floats, scale_mat_data, error));
  }

  // Scale
  {
    const math::vec3 scale_vec = math::vec3_init(1.1f, 2.2f, 3.3f);
    const math::mat4 scale_with_vec = math::mat4_scale(scale_vec);
    CHECK(mat4_equal(scale_with_vec, scale_mat_data, error));  
  }
}


TEST(mat4_translate)
{
  // Translate
  {
    const math::mat4 trans_with_floats = math::mat4_translate(10, 12, 14);
    CHECK(mat4_equal(trans_with_floats, trans_mat_data, error));
  }

  // Translate
  {
    const math::vec3 trans_vec = math::vec3_init(10.f, 12.f, 14.f);
    const math::mat4 trans_with_vec = math::mat4_translate(trans_vec);
    CHECK(mat4_equal(trans_with_vec, trans_mat_data, error));
  }
}


TEST(mat4_rotation)
{
  // Rotate
  {
    const math::vec3 axis             = math::vec3_init(0, 1, 0);
    const float rotate_rads           = math::pi();
    const math::mat4 rot_mat          = math::mat4_rotate_around_axis(axis, rotate_rads);
    const math::vec4 point_to_rotate  = math::vec4_init(1, 0, 0, 0);
    const math::vec4 result_vec       = math::mat4_multiply(point_to_rotate, rot_mat);

    CHECK(vec4_components_are_near(result_vec, -1, 0, 0, 0, error));
  }

  // Rotate
  {
    const math::vec3 axis             = math::vec3_init(1, 0, 0);
    const float rotate_rads           = math::pi();
    const math::mat4 rot_mat          = math::mat4_rotate_around_axis(axis, rotate_rads);
    const math::vec4 point_to_rotate  = math::vec4_init(1, 0, 0, 0);
    const math::vec4 result_vec       = math::mat4_multiply(point_to_rotate, rot_mat);

    CHECK(vec4_components_are_near(result_vec, 1, 0, 0, 0, error));
  }

  // Rotate
  {
    const math::vec3 axis             = math::vec3_init(0, 1, 0);
    const float rotate_rads           = math::tau();
    const math::mat4 rot_mat          = math::mat4_rotate_around_axis(axis, rotate_rads);
    const math::vec4 point_to_rotate  = math::vec4_init(0, 0, 1, 0);
    const math::vec4 result_vec       = math::mat4_multiply(point_to_rotate, rot_mat);

    CHECK(vec4_components_are_near(result_vec, 0, 0, -1, 0, error));
    std::cout << math::vec4_get_x(result_vec) << ", " << math::vec4_get_y(result_vec) << ", " << math::vec4_get_z(result_vec) << ", " << math::vec4_get_z(result_vec) << std::endl;
  }  
}


TEST(mat4_arithmetic)
{
  const math::mat4 one_mat  = math::mat4_init_with_array(one_mat_data);
  const math::mat4 two_mat  = math::mat4_init_with_array(two_mat_data);

  const math::mat4 add_mat = math::mat4_add(one_mat, one_mat);
  CHECK(mat4_equal(add_mat, two_mat_data, error));

  const math::mat4 sub_mat = math::mat4_subtract(one_mat, one_mat);
  CHECK(mat4_equal(sub_mat, zero_mat_data, error));


  const math::vec4 two_vec4 = math::vec4_init(2, 2, 2, 2);
  const math::vec3 two_vec3 = math::vec3_init(2, 2, 2);

  //const math::mat4 scale_mat = math::mat4_scale(0.5f, two_mat);
  //CHECK(mat4_equal(scale_mat, one_mat_data, error));

  const math::vec4 mul_with_vec = math::mat4_multiply(two_vec4, one_mat);
  CHECK(false);

  const math::mat4 mul_mat = math::mat4_multiply(two_mat, two_mat);
  CHECK(mat4_equal(mul_mat, sixteen_mat_data, error));
}


int main()
{
  test::run_tests();

  return 0;
}