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

		std::cout << caffmath::matrix44_get(mat, i) << ", " << mat_data.at(i) << std::endl;
		std::cout << diff << "," << abs_diff << std::endl;

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

	const std::array<float, 16> two_mat_data = {
		2,2,2,2,
		2,2,2,2,
		2,2,2,2,
		2,2,2,2,
	};	

	const std::array<float, 16> id_mat_data = {
		2,0,0,0,
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

	// 

	return 0;
}