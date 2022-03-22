#include "Axies.h"

Axies::Axies(std::vector<glm::fvec3> p) : m_shader(Shader("assets/shaders/axies.vs.glsl", "assets/shaders/axies.fs.glsl")),
	lineColor(glm::fvec3(1.0f)),
	MVP(glm::mat4(1.0f)) {

	points.resize(6);
	points[0] = p[0];
	points[1] = p[1];
	points[2] = p[2];
	points[3] = p[3];
	points[4] = p[4];
	points[5] = p[5];

	vertices = {
		points[0].x, points[0].y, points[0].z,
		points[1].x, points[1].y, points[1].z,
		points[2].x, points[2].y, points[2].z,
		points[3].x, points[3].y, points[3].z,
		points[4].x, points[4].y, points[4].z,
		points[5].x, points[5].y, points[5].z,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Axies::~Axies()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(m_shader.ID);
}

void Axies::setMVP(glm::fmat4 mvp)
{
	MVP = mvp;
}

void Axies::setLineColor(glm::fvec3 color)
{
	lineColor = color;
}

void Axies::draw()
{
	m_shader.use();

	m_shader.setMat4("MVP", MVP);

	m_shader.setVec3("color", lineColor);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glDrawArrays(GL_LINE_STRIP, 2, 2);
	glDrawArrays(GL_LINE_STRIP, 4, 2);
}
