#ifndef SHAPE_H
#define SHAPE_H

#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw(GLuint program, const glm::mat4& vpMatrix) = 0;
    virtual void updateColors() = 0;
    virtual void setRotation(float angleX, float angleY) = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void setupBuffers() = 0;

protected:
    virtual void generateRandomColors() = 0;

    GLuint vao, vbo, ebo;
    glm::mat4 modelMatrix;
    float rotationX;  // Член класса для хранения угла вращения по оси X
    float rotationY;  // Член класса для хранения угла вращения по оси Y
    glm::vec3 pos;
};

#endif // SHAPE_H
