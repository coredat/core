#ifndef TEST_ASSERTS_INCLUDED_E2F2BA2B_C021_4A90_8BFD_945C128208AB
#define TEST_ASSERTS_INCLUDED_E2F2BA2B_C021_4A90_8BFD_945C128208AB


#include "../math/vector/vector.hpp"
#include <iostream>


void assert_test(const std::string &test, const bool passed)
{
	if(!passed)
	{
		std::cout << test << " - failed!" << std::endl;
	}
}

// Vector 4

bool vector4_is_equal(const caffmath::vector4 a, const caffmath::vector4 b)
{
	if(caffmath::vector4_get_x(a) == caffmath::vector4_get_x(b) &&
	   caffmath::vector4_get_y(a) == caffmath::vector4_get_y(b) &&
	   caffmath::vector4_get_z(a) == caffmath::vector4_get_z(b))
	{
		return true;
	}

	return false;
}

bool vector4_is_not_equal(const caffmath::vector4 a, const caffmath::vector4 b)
{
	return !vector4_is_equal(a, b);
}

bool vector4_components_are(const caffmath::vector4 a, const float x, const float y, const float z, const float w)
{
	if(caffmath::vector4_get_x(a) == x && caffmath::vector4_get_y(a) == y && caffmath::vector4_get_z(a) == z && caffmath::vector4_get_w(a) == w)
	{
		return true;
	}

	//std::cout << caffmath::vector4_get_x(a) << ", " << caffmath::vector4_get_y(a) << ", " << caffmath::vector4_get_z(a) << ", " << caffmath::vector4_get_w(a) << std::endl;

	return false;
}

bool vector4_components_are_near(const caffmath::vector4 a, const float x, const float y, const float z, const float w, const float error)
{
	const float x_result = caffmath::vector4_get_x(a) - x;
	const float y_result = caffmath::vector4_get_y(a) - y;
	const float z_result = caffmath::vector4_get_z(a) - z;
	const float w_result = caffmath::vector4_get_w(a) - w;

	if(std::abs(x_result) < error && std::abs(y_result) < error && std::abs(z_result) < error && std::abs(w_result) < error)
	{
		return true;
	}

	std::cout << caffmath::to_string(a) << std::endl;

	return false;
}


// Vector3
bool vector3_is_equal(const caffmath::vector3 a, const caffmath::vector3 b)
{
	if(caffmath::vector3_get_x(a) == caffmath::vector3_get_x(b) &&
	   caffmath::vector3_get_y(a) == caffmath::vector3_get_y(b))
	{
		return true;
	}

	return false;
}

bool vector3_is_not_equal(const caffmath::vector3 a, const caffmath::vector3 b)
{
	return !vector3_is_equal(a, b);
}

bool vector3_components_are(const caffmath::vector3 a, const float x, const float y, const float z)
{
	if(caffmath::vector3_get_x(a) == x && caffmath::vector3_get_y(a) == y && caffmath::vector3_get_z(a) == z)
	{
		return true;
	}

	//std::cout << caffmath::vector4_get_x(a) << ", " << caffmath::vector4_get_y(a) << ", " << caffmath::vector4_get_z(a) << ", " << caffmath::vector4_get_w(a) << std::endl;

	return false;
}

bool vector3_components_are_near(const caffmath::vector3 a, const float x, const float y, const float z, float error)
{
	const float x_result = caffmath::vector3_get_x(a) - x;
	const float y_result = caffmath::vector3_get_y(a) - y;
	const float z_result = caffmath::vector3_get_z(a) - z;

	if(std::abs(x_result) < error && std::abs(y_result) < error && std::abs(z_result) < error)
	{
		return true;
	}

	std::cout << caffmath::to_string(a) << std::endl;

	return false;
}

// Vector 2
bool vector2_is_equal(const caffmath::vector2 a, const caffmath::vector2 b)
{
	if(caffmath::vector2_get_x(a) == caffmath::vector2_get_x(b) &&
	   caffmath::vector2_get_y(a) == caffmath::vector2_get_y(b))
	{
		return true;
	}

	return false;
}

bool vector2_is_not_equal(const caffmath::vector2 a, const caffmath::vector2 b)
{
	return !vector2_is_equal(a, b);
}

bool vector2_components_are(const caffmath::vector2 a, const float x, const float y)
{
	if(caffmath::vector2_get_x(a) == x && caffmath::vector2_get_y(a) == y)
	{
		return true;
	}

	//std::cout << caffmath::vector4_get_x(a) << ", " << caffmath::vector4_get_y(a) << ", " << caffmath::vector4_get_z(a) << ", " << caffmath::vector4_get_w(a) << std::endl;

	return false;
}

bool vector2_components_are_near(const caffmath::vector2 a, const float x, const float y, float error)
{
	const float x_result = caffmath::vector2_get_x(a) - x;
	const float y_result = caffmath::vector2_get_y(a) - y;

	if(std::abs(x_result) < error && std::abs(y_result) < error)
	{
		return true;
	}

	std::cout << caffmath::to_string(a) << std::endl;

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