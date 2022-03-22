#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Conversion.h"

#include <utility>
#include <tuple>
#include <iostream>

static class Isometries
{
public:
	static glm::fmat3 rodrigues(glm::fvec3 p, float phi);
	static glm::fmat3 euler_2a(float phi, float theta, float psi);
	static std::pair<Eigen::Vector3f, float> axis_angle(Eigen::Matrix3f A);
	static std::tuple<float, float, float> a2_euler(glm::fmat3 A);
	static glm::quat axis_engle_2q(glm::fvec3 p, float phi);
	static std::pair<glm::fvec3, float> q2_axis_angle(glm::quat q);
	static void print(glm::fmat3);
	static void print_tuple(std::tuple<float, float, float>);
	static glm::quat slerp(glm::quat q1, glm::quat q2, float tm, float t);
	static void test1();
	static void my_test();
};

