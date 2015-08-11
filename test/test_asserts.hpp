#ifndef TEST_ASSERTS_INCLUDED_E2F2BA2B_C021_4A90_8BFD_945C128208AB
#define TEST_ASSERTS_INCLUDED_E2F2BA2B_C021_4A90_8BFD_945C128208AB

/*
	These asserts are for unit tests.
*/

#include <math/vec/vec.hpp>
#include <math/mat/mat.hpp>
#include <math/general/to_string.hpp>
#include <iostream>


bool
mat4_equal(math::mat4 mat, const std::array<float, 16> mat_data, const float error)
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


// vec 4

bool vec4_is_equal(const math::vec4 a, const math::vec4 b)
{
	if(math::vec4_get_x(a) == math::vec4_get_x(b) &&
	   math::vec4_get_y(a) == math::vec4_get_y(b) &&
	   math::vec4_get_z(a) == math::vec4_get_z(b))
	{
		return true;
	}

	return false;
}

bool vec4_is_not_equal(const math::vec4 a, const math::vec4 b)
{
	return !vec4_is_equal(a, b);
}

bool vec4_components_are(const math::vec4 a, const float x, const float y, const float z, const float w)
{
	if(math::vec4_get_x(a) == x && math::vec4_get_y(a) == y && math::vec4_get_z(a) == z && math::vec4_get_w(a) == w)
	{
		return true;
	}

	//std::cout << math::vec4_get_x(a) << ", " << math::vec4_get_y(a) << ", " << math::vec4_get_z(a) << ", " << math::vec4_get_w(a) << std::endl;

	return false;
}

bool vec4_components_are_near(const math::vec4 a, const float x, const float y, const float z, const float w, const float error)
{
	const float x_result = math::vec4_get_x(a) - x;
	const float y_result = math::vec4_get_y(a) - y;
	const float z_result = math::vec4_get_z(a) - z;
	const float w_result = math::vec4_get_w(a) - w;

	if(std::abs(x_result) < error && std::abs(y_result) < error && std::abs(z_result) < error && std::abs(w_result) < error)
	{
		return true;
	}

	std::cout << math::to_string(a) << std::endl;

	return false;
}


// vec3
bool vec3_is_equal(const math::vec3 a, const math::vec3 b)
{
	if(math::vec3_get_x(a) == math::vec3_get_x(b) &&
	   math::vec3_get_y(a) == math::vec3_get_y(b))
	{
		return true;
	}

	return false;
}

bool vec3_is_not_equal(const math::vec3 a, const math::vec3 b)
{
	return !vec3_is_equal(a, b);
}

bool vec3_components_are(const math::vec3 a, const float x, const float y, const float z)
{
	if(math::vec3_get_x(a) == x && math::vec3_get_y(a) == y && math::vec3_get_z(a) == z)
	{
		return true;
	}

	//std::cout << math::vec4_get_x(a) << ", " << math::vec4_get_y(a) << ", " << math::vec4_get_z(a) << ", " << math::vec4_get_w(a) << std::endl;

	return false;
}

bool vec3_components_are_near(const math::vec3 a, const float x, const float y, const float z, float error)
{
	const float x_result = math::vec3_get_x(a) - x;
	const float y_result = math::vec3_get_y(a) - y;
	const float z_result = math::vec3_get_z(a) - z;

	if(std::abs(x_result) < error && std::abs(y_result) < error && std::abs(z_result) < error)
	{
		return true;
	}

	std::cout << math::to_string(a) << std::endl;

	return false;
}

// vec 2
bool vec2_is_equal(const math::vec2 a, const math::vec2 b)
{
	if(math::vec2_get_x(a) == math::vec2_get_x(b) &&
	   math::vec2_get_y(a) == math::vec2_get_y(b))
	{
		return true;
	}

	return false;
}

bool vec2_is_not_equal(const math::vec2 a, const math::vec2 b)
{
	return !vec2_is_equal(a, b);
}

bool vec2_components_are(const math::vec2 a, const float x, const float y)
{
	if(math::vec2_get_x(a) == x && math::vec2_get_y(a) == y)
	{
		return true;
	}

	//std::cout << math::vec4_get_x(a) << ", " << math::vec4_get_y(a) << ", " << math::vec4_get_z(a) << ", " << math::vec4_get_w(a) << std::endl;

	return false;
}

bool vec2_components_are_near(const math::vec2 a, const float x, const float y, float error)
{
	const float x_result = math::vec2_get_x(a) - x;
	const float y_result = math::vec2_get_y(a) - y;

	if(std::abs(x_result) < error && std::abs(y_result) < error)
	{
		return true;
	}

	std::cout << math::to_string(a) << std::endl;

	return false;
}


bool float_is_near(const float a, const float b, const float error)
{
	const float result = a - b;
	if(std::abs(result) < error)
	{
		return true;
	}

	//std::cout << a << std::endl;

	return false;
}


#endif