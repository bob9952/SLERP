#pragma once
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Axies
{
public:
	Axies(std::vector<glm::fvec3>);
	~Axies();
    void setMVP(glm::fmat4 model);
    void setLineColor(glm::fvec3 color);
    void draw();
private:
    Shader m_shader;
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    std::vector<glm::fvec3> points;
    glm::fvec3 lineColor;
    glm::fmat4 MVP;
};

