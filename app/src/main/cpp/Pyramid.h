#ifndef PYRAMID_H
#define PYRAMID_H

#include "Shape.h"

class Pyramid : public Shape {
public:
    Pyramid();
    ~Pyramid() override;
    void draw(GLuint program, const glm::mat4& vpMatrix) override;
    void updateColors() override;
    void setRotation(float angleX, float angleY) override;
    void setPosition(const glm::vec3& position) override;
    void setupBuffers() override;

protected:
    void generateRandomColors() override;

private:
    static const int numVertices = 16;
    float vertices[numVertices * 9];
    GLushort indices[18] = {
            0, 1, 2, 0, 2, 3,  // base
            0, 1, 4,           // front face
            1, 2, 4,           // right face
            2, 3, 4,           // back face
            3, 0, 4            // left face
    };
};

#endif // PYRAMID_H
