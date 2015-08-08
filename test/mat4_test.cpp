// Test for mat4
#include "../math/mat/mat4.hpp"
#include "../math/general/to_string.hpp"
#include "test_asserts.hpp"
#include <unit_test/unit_test.hpp>
#include <iostream>
#include <cmath>


bool mat4_equal(math::mat4 mat, const std::array<float, 16> mat_data, const float error)
{
	for(uint32_t i = 0; i < mat_data.size(); ++i)
	{
		const float diff = math::mat4_get(mat, i) - mat_data.at(i);
		const float abs_diff = math::abs(diff);

		//std::cout << math::mat4_get(mat, i) << ", " << mat_data.at(i) << std::endl;
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
	std::cout << "mat4 Test" << std::endl;
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
		const math::mat4 id_mat = math::mat4_id();
		CHECK("matrix is id", mat4_equal(id_mat, id_mat_data, error));

		const math::mat4 zero_mat = math::mat4_zero();
		CHECK("matrix is zero", mat4_equal(zero_mat, zero_mat_data, error));
	}

	// Init Mat
	{
		const math::mat4 init_c_array = math::mat4_init_with_array(&sequence_mat_data[0]);
		CHECK("matrix c-array init", mat4_equal(init_c_array, sequence_mat_data, error));

		const math::mat4 init_std_array = math::mat4_init_with_array(sequence_mat_data);
		CHECK("matrix c-array init", mat4_equal(init_std_array, sequence_mat_data, error));

		const math::mat4 init_with_no_value = math::mat4_init();
		CHECK("matix no init value", mat4_equal(init_with_no_value, id_mat_data, error));

		const math::mat4 init_with_value = math::mat4_init(2);
		CHECK("matix init value", mat4_equal(init_with_value, two_mat_data, error));		
	}

	// special mats
	{
		// Look at
		const math::vec3 position = math::vec3_zero();
		const math::vec3 target = math::vec3_init(1,1,1);
		const math::vec3 up = math::vec3_init(0,1,0);

		const math::mat4 look_at_mat = math::mat4_lookat(position, target, up);
		CHECK("lookat matrix", false);

		// Projection
		const float width = 800.f;
		const float height = 600.f;
		const float near_plane = 1.f;
		const float far_plane = 100.f;
		const float field_of_view = math::quart_tau();

		const math::mat4 proj_mat = math::mat4_projection(width, height, near_plane, far_plane, field_of_view);
		CHECK("proj matrix", false);

		// Orth
		CHECK("ortho matrix", false);
	}

	// Operations
	{
		const math::mat4 one_mat = math::mat4_init_with_array(one_mat_data);
		const math::mat4 two_mat = math::mat4_init_with_array(two_mat_data);
		const math::vec4 two_vec4 = math::vec4_init(2, 2, 2, 2);
		const math::vec3 two_vec3 = math::vec3_init(2, 2, 2);

		const math::mat4 add_mat = math::mat4_add(one_mat, one_mat);
		CHECK("add matrix", mat4_equal(one_mat, two_mat_data, error));

		const math::mat4 sub_mat = math::mat4_subtract(one_mat, one_mat);
		CHECK("sub matrix", mat4_equal(sub_mat, zero_mat_data, error));

		const math::mat4 scale_mat = math::mat4_scale(0.5f, two_mat);
		CHECK("scale matrix", mat4_equal(scale_mat, one_mat_data, error));

		const math::vec4 mul_with_vec = math::mat4_multiply(two_vec4, one_mat);
		CHECK("mul with vec", false);

		const math::mat4 mul_mat = math::mat4_multiply(two_mat, two_mat);
		CHECK("mul with mat", mat4_equal(mul_mat, sixteen_mat_data, error));

		const math::mat4 trans_mat = math::mat4_translate(two_mat, two_vec3);
		CHECK("translate", mat4_equal(trans_mat, trans_mat_data, error));

		CHECK("rot", false);
	}

	// Convert to other forms
	{

	}

	//
	{
		
	} 

	return 0;
}