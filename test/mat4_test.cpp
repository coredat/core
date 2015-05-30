// Test for vector2
#include "../math/matrix/matrix44.hpp"
#include "../math/general/to_string.hpp"
#include "test_asserts.hpp"
#include <iostream>
#include <cmath>


bool matrix44_equal(caffmath::matrix44 mat, const std::array<float, 16> mat_data, const float error)
{
	for(uint32_t i = 0; i < mat_data.size(); ++i)
	{
		const float diff = caffmath::matrix44_get(mat, i) - mat_data.at(i);
		const float abs_diff = caffmath::abs(diff);

		//std::cout << caffmath::matrix44_get(mat, i) << ", " << mat_data.at(i) << std::endl;
		//std::cout << diff << "," << abs_diff << std::endl;

		if(diff > error)
		{
			return false;
		}
	}

	return true;
}


int main()
{
	std::cout << "Matrix44 Test" << std::endl;
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

	const std::array<float, 16> sequence_mat_data = {
		1.f,1.1f,1.2f,1.3f,
		2.f,2.1f,2.2f,2.3f,
		3.f,3.1f,3.2f,3.3f,
		4.f,4.1f,4.2f,4.3f,
	};

	const std::array<float, 16> trans_mat_data = {
		2,2,2,2,
		2,2,2,2,
		2,2,2,2,
		4,4,4,2,
	};

	// Mat Constants
	{
		const caffmath::matrix44 id_mat = caffmath::matrix44_id();
		assert_test("matrix is id", matrix44_equal(id_mat, id_mat_data, error));

		const caffmath::matrix44 zero_mat = caffmath::matrix44_zero();
		assert_test("matrix is zero", matrix44_equal(zero_mat, zero_mat_data, error));
	}

	// Init Mat
	{
		const caffmath::matrix44 init_c_array = caffmath::matrix44_init_with_array(&sequence_mat_data[0]);
		assert_test("matrix c-array init", matrix44_equal(init_c_array, sequence_mat_data, error));

		const caffmath::matrix44 init_std_array = caffmath::matrix44_init_with_array(sequence_mat_data);
		assert_test("matrix c-array init", matrix44_equal(init_std_array, sequence_mat_data, error));

		const caffmath::matrix44 init_with_no_value = caffmath::matrix44_init();
		assert_test("matix no init value", matrix44_equal(init_with_no_value, id_mat_data, error));

		const caffmath::matrix44 init_with_value = caffmath::matrix44_init(2);
		assert_test("matix init value", matrix44_equal(init_with_value, two_mat_data, error));		
	}

	// special mats
	{
		// Look at
		const caffmath::vector3 position = caffmath::vector3_zero();
		const caffmath::vector3 target = caffmath::vector3_init(1,1,1);
		const caffmath::vector3 up = caffmath::vector3_init(0,1,0);

		const caffmath::matrix44 look_at_mat = caffmath::matrix44_init_lookat(position, target, up);
		assert_test("lookat matrix", false);

		// Projection
		const float width = 800.f;
		const float height = 600.f;
		const float near_plane = 1.f;
		const float far_plane = 100.f;
		const float field_of_view = caffmath::quart_tau();

		const caffmath::matrix44 proj_mat = caffmath::matrix44_init_projection(width, height, near_plane, far_plane, field_of_view);
		assert_test("proj matrix", false);

		// Orth
		assert_test("ortho matrix", false);
	}

	// Operations
	{
		const caffmath::matrix44 one_mat = caffmath::matrix44_init_with_array(one_mat_data);
		const caffmath::matrix44 two_mat = caffmath::matrix44_init_with_array(two_mat_data);
		const caffmath::vector4 two_vec4 = caffmath::vector4_init(2, 2, 2, 2);
		const caffmath::vector3 two_vec3 = caffmath::vector3_init(2, 2, 2);

		const caffmath::matrix44 add_mat = caffmath::matrix44_add(one_mat, one_mat);
		assert_test("add matrix", matrix44_equal(one_mat, two_mat_data, error));

		const caffmath::matrix44 sub_mat = caffmath::matrix44_subtract(one_mat, one_mat);
		assert_test("sub matrix", matrix44_equal(sub_mat, zero_mat_data, error));

		const caffmath::matrix44 scale_mat = caffmath::matrix44_scale(0.5f, two_mat);
		assert_test("scale matrix", matrix44_equal(scale_mat, one_mat_data, error));

		const caffmath::vector4 mul_with_vec = caffmath::matrix44_multiply(two_vec4, one_mat);
		assert_test("mul with vec", false);

		const caffmath::matrix44 mul_mat = caffmath::matrix44_multiply(two_mat, two_mat);
		assert_test("mul with mat", matrix44_equal(mul_mat, sixteen_mat_data, error));

		const caffmath::matrix44 trans_mat = caffmath::matrix44_translate(two_mat, two_vec3);
		assert_test("translate", matrix44_equal(trans_mat, trans_mat_data, error));

		assert_test("rot", false);
	}

	// Convert to other forms
	{

	}

	//
	{
		
	} 

	return 0;
}