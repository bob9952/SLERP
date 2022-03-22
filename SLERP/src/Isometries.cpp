#include "Isometries.h"

glm::fmat3 Isometries::rodrigues(glm::fvec3 p, float phi)
{
	float n = glm::l2Norm(p);
	p = p / n;
	
	// 3 x 1    *   1 x 3 
	glm::fmat3 m(p.x * p.x, p.y * p.x, p.z * p.x,
				p.x * p.y, p.y * p.y, p.z * p.y,
				p.x * p.z, p.y * p.z, p.z * p.z);

	float sinfi = glm::sin(phi);
	float cosfi = glm::cos(phi);

	glm::fmat3 E = glm::diagonal3x3(glm::vec3(1.0f));
	glm::fmat3 px(0.0f, p.z, -p.y, -p.z, 0.0f, p.x, p.y, -p.x, 0.0f);


	glm::fmat3 matrix = m + cosfi * (E - m) + sinfi * px;
	return matrix;
}

glm::fmat3 Isometries::euler_2a(float phi, float theta, float psi)
{
	glm::fmat3 Rx(1.0f, 0.0f, 0.0f, 0.0f, glm::cos(phi), glm::sin(phi), 0.0f, -glm::sin(phi), glm::cos(phi));
	glm::fmat3 Ry(glm::cos(theta), 0.0f, -glm::sin(theta), 0.0f, 1.0f, 0.0f, glm::sin(theta), 0.0f, glm::cos(theta));
	glm::fmat3 Rz(glm::cos(psi), glm::sin(psi), 0.0f , -glm::sin(psi), glm::cos(psi), 0.0f, 0.0f, 0.0f, 1.0f);

	return Rz * Ry* Rx;
}

std::pair<Eigen::Vector3f, float> Isometries::axis_angle(Eigen::Matrix3f A)
{
	Eigen::EigenSolver<Eigen::MatrixXf> eigensolver;

	eigensolver.compute(A);
	Eigen::VectorXf eigen_values = eigensolver.eigenvalues().real();
	Eigen::MatrixXf eigen_vectors = eigensolver.eigenvectors().real();
	Eigen::MatrixXf tmp;
	tmp.resize(3, 1);
	tmp << eigen_values;

	//std::cout << "EIGEN VLAUES: " << std::endl << eigen_values.transpose() << std::endl;
	//std::cout << "EIGEN VECTORS: "  << std::endl << eigen_vectors.transpose() << std::endl;
	
	Eigen::Vector3f p;
	for (int i = 0; i < eigen_values.rows(); i++) {
		float value = roundf(eigen_values(i) * 1000.0f) / 1000.0f;
		if (value == 1.0f) {
			p = eigen_vectors.col(i);
		}
	}

	//std::cout << "p: " << p << std::endl;
	
	
	auto u = p.cross(Eigen::Vector3f(p(0), p(1), -p(2)));
	u.normalize();

	//std::cout << "u: " << u << std::endl;

	auto up = A * u;
	float angle = std::acos(u.dot(up));

	//std::cout << "up: " << up << std::endl;
	//std::cout << "angle: " << angle << std::endl;

	Eigen::Matrix3f m;
	m.row(0) = u;
	m.row(1) = up;
	m.row(2) = p;

	//std::cout << "m: " << m << std::endl;
	
	if (m.determinant() < 0.0f) {
		p = -p;
	}
	
	return std::pair<Eigen::Vector3f, float>(p, angle);
}

std::tuple<float, float, float> Isometries::a2_euler(glm::fmat3 A)
{
	float phi = 0.0f;
	float theta = 0.0f;
	float psi = 0.0f;

	// AA^T = A^TA = E 
	// A^T = A^(-1)
	//std::cout << std::endl;
	//std::cout << "Transpose of A:" << std::endl;
	//Isometries::print(glm::transpose(A));
	//std::cout << "Inverse of A:" << std::endl;
	//Isometries::print(glm::inverse(A));
	//std::cout << std::endl;

	/* Check if two matrices are same 
	* How to do this???
	if (glm::epsilonEqual(glm::transpose(A), glm::inverse(A), 0.000001f)) {
		std::cout << "Not a orthogonal matrix" << std::endl;
		return std::tuple<float, float, float>();
	}
	*/
	if (std::fabs(A[2][0]) - 1.0f < FLT_EPSILON) {
		if (A[2][0] + 1.0f > FLT_EPSILON) {
			psi = glm::atan(A[1][0], A[0][0]);
			theta = glm::asin(-A[2][0]);
			phi = glm::atan(A[2][1], A[2][2]);
		}else {
			psi = glm::atan(-A[0][1], A[1][1]);
			theta = glm::pi<float>() / 2.0f;
			phi = 0.0f;
		}
	}
	else {
		phi = glm::atan(A[2][1], A[2][2]);
		theta = glm::asin(-A[2][0]);
		psi = glm::atan(A[1][0], A[0][0]);
	}

	return std::tuple<float, float, float>(phi, theta, psi);
}

glm::quat Isometries::axis_engle_2q(glm::fvec3 p, float phi)
{
	float w = glm::cos(phi / 2);
	p = glm::normalize(p);

	auto tmp = glm::sin(phi / 2) * p;
	return glm::quat(w, tmp.x, tmp.y, tmp.z);
}

std::pair<glm::fvec3, float> Isometries::q2_axis_angle(glm::quat q)
{
	q = glm::normalize(q);

	glm::fvec3 p;
	float phi = 2 * glm::acos(q.w);
	
	if (glm::abs(q.w-1.0f) < 0.0001f) {
		p = glm::fvec3(1.0f, 0.0f, 0.0f);
	}
	else {
		p = glm::fvec3(q.x, q.y, q.z);
		p = glm::normalize(p);
	}

	return std::pair<glm::fvec3, float>(p, phi);
}

void Isometries::print(glm::fmat3 result)
{
	float fArray[9] = { 0.0f };

	const float* pSource = (const float*)glm::value_ptr(result);
	for (int i = 0; i < 9; ++i)
		fArray[i] = pSource[i];

	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0)
			std::cout << std::endl;
		std::cout << fArray[i] << "\t";
	}
	std::cout << std::endl << std::endl;
}

void Isometries::print_tuple(std::tuple<float, float, float> t)
{
	std::cout << "phi: " << std::get<0>(t) << std::endl;
	std::cout << "theta: " << std::get<1>(t) << std::endl;
	std::cout << "psi: " << std::get<2>(t) << std::endl;
	std::cout << std::endl;
}

glm::quat Isometries::slerp(glm::quat q1, glm::quat q2, float tm, float t)
{
	if (t < 0 || t > tm) {
		std::cout << "t out of bounds" << std::endl;
	}

	q1 = glm::normalize(q1);
	q2 = glm::normalize(q2);

	auto cos0 = glm::dot(q1, q2);
	if (cos0 < 0.00001f) {
		q1 = -q1;
		cos0 = -cos0;
	}

	if (cos0 > 0.99999f) {
		return q1;
	}

	auto phi0 = glm::acos(cos0);
	
	auto qs = (glm::sin(phi0 * (1.0f - t / tm)) / glm::sin(phi0)) * q1 
		+ (glm::sin(phi0 * t / tm) / glm::sin(phi0)) * q2;

	return qs;
}

void Isometries::test1()
{
	constexpr float pi = glm::pi<float>();

	float phi = -glm::atan(1.0f / 4.0f);
	float theta = -glm::asin(8.0f / 9.0f);
	float psi = glm::atan(4.0f);

	std::cout << "PHI: " << phi << std::endl;
	std::cout << "THETA: " << theta << std::endl;
	std::cout << "PSI: " << psi << std::endl << std::endl;


	glm::fmat3 A = Isometries::euler_2a(phi, theta, psi);
	A = glm::transpose(A);
	std::cout << "Euler2A[phi, theta, psi]" << std::endl;
	Isometries::print(A);

	auto A_eigen = GLM2E(A);
	auto res = Isometries::axis_angle(A_eigen);
	std::cout << "AxisAngle[A]" << std::endl;
	std::cout << "p: " << res.first(0) << " " << res.first(1) << " "
		<< res.first(2) << "\t" << "angle: " << res.second << std::endl << std::endl;


	float angle = res.second;
	auto p = glm::fvec3(res.first(0), res.first(1), res.first(2));

	glm::fmat3 result = Isometries::rodrigues(p, angle);
	
	std::cout << "Rodrigez[p, phi]" << std::endl;
	Isometries::print(result);


	auto res_a2_euler = Isometries::a2_euler(A);
	std::cout << "A2Euler[A]" << std::endl << std::endl;
	Isometries::print_tuple(res_a2_euler);

	auto q = Isometries::axis_engle_2q(p, angle);
	std::cout << "AxisAngle2Q[p, phi] " << std::endl << std::endl;
	std::cout << "quaternion q: " << glm::to_string(q) << std::endl << std::endl;

	auto res_q2 = Isometries::q2_axis_angle(q);
	std::cout << "Q2AxisAngle[q]" << std::endl << std::endl;
	std::cout << "p: " << glm::to_string(res_q2.first) << std::endl;
	std::cout << "phi: " << res_q2.second << std::endl;

}

void Isometries::my_test()
{
	constexpr float pi = glm::pi<float>();

	float phi = -pi / 6.0f;
	float theta = - pi / 6.0f;
	float psi = pi / 6.0f;

	std::cout << "PHI: " << phi << std::endl;
	std::cout << "THETA: " << theta << std::endl;
	std::cout << "PSI: " << psi << std::endl << std::endl;


	glm::fmat3 A = Isometries::euler_2a(phi, theta, psi);
	A = glm::transpose(A);
	std::cout << "Euler2A[phi, theta, psi]" << std::endl;
	Isometries::print(A);

	auto A_eigen = GLM2E(A);
	auto res = Isometries::axis_angle(A_eigen);
	std::cout << "AxisAngle[A]" << std::endl;
	std::cout << "p: " << res.first(0) << " " << res.first(1) << " "
		<< res.first(2) << "\t" << "angle: " << res.second << std::endl << std::endl;


	float angle = res.second;
	auto p = glm::fvec3(res.first(0), res.first(1), res.first(2));

	glm::fmat3 result = Isometries::rodrigues(p, angle);

	std::cout << "Rodrigez[p, phi]" << std::endl;
	Isometries::print(result);


	auto res_a2_euler = Isometries::a2_euler(A);
	std::cout << "A2Euler[A]" << std::endl << std::endl;
	Isometries::print_tuple(res_a2_euler);

	auto q = Isometries::axis_engle_2q(p, angle);
	std::cout << "AxisAngle2Q[p, phi] " << std::endl << std::endl;
	std::cout << "quaternion q: " << glm::to_string(q) << std::endl << std::endl;

	auto res_q2 = Isometries::q2_axis_angle(q);
	std::cout << "Q2AxisAngle[q]" << std::endl << std::endl;
	std::cout << "p: " << glm::to_string(res_q2.first) << std::endl;
	std::cout << "phi: " << res_q2.second << std::endl;

}