#pragma once

#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Line
{
public:
    Line(glm::vec3 start, glm::vec3 end);
    ~Line();
    void setModel(glm::mat4 model);
    void setView(glm::mat4 view);
    void setProjection(glm::mat4 projection);
    void setLineColor(glm::vec3 color);
    void draw();
private:
    Shader m_shader;
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    glm::vec3 lineColor;
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;
};

