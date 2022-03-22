#pragma once
#include <Eigen/Dense>
#include <glm/matrix.hpp>

template<typename T, int m, int n>
inline glm::mat<m, n, float> E2GLM(Eigen::Matrix<T, m, n>& em)
{
	glm::mat<m, n, float> mat;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			mat[j][i] = em(i, j);
		}
	}
	return mat;
}

template<typename T, int m>
inline glm::vec<m, float> E2GLM(Eigen::Matrix<T, m, 1>& em)
{
	glm::vec<m, float> v;
	for (int i = 0; i < m; ++i)
	{
		v[i] = em(i);
	}
	return v;
}


inline Eigen::Matrix3f GLM2E(glm::fmat3& glmm) {
	Eigen::Matrix3f result;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			result(i,j) = glmm[j][i];
		}
	}
	return result;
}