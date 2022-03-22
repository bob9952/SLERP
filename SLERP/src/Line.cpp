#include "Line.h"

Line::Line(glm::vec3 start, glm::vec3 end) :
	m_shader(Shader("assets/shaders/line.vs.glsl", "assets/shaders/line.fs.glsl")),
	startPoint(start),
	endPoint(end), 
	lineColor(glm::vec3(1.0f)),
	m_model(glm::mat4(1.0f)),
	m_view(glm::mat4(1.0f)),
	m_projection(glm::mat4(1.0f))
{
	vertices = {
		startPoint.x, startPoint.y, startPoint.z,
		endPoint.x, endPoint.y, endPoint.z
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Line::~Line()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(m_shader.ID);
}

void Line::setModel(glm::mat4 model)
{
	m_model = model;
}

void Line::setView(glm::mat4 view)
{
	m_view = view;
}

void Line::setProjection(glm::mat4 projection)
{
	m_projection = projection;
}

void Line::setLineColor(glm::vec3 color)
{
	lineColor = color;
}

void Line::draw()
{
	m_shader.use();

	m_shader.setMat4("projection", m_projection);
	m_shader.setMat4("view", m_view);
	m_shader.setMat4("model", m_model);

	m_shader.setVec3("color", lineColor);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
}
