#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
public:
    Cube();
    ~Cube() override;
    void draw(GLuint program, const glm::mat4& vpMatrix) override;
    void updateColors() override;
    void setRotation(float angleX, float angleY) override;
    void setPosition(const glm::vec3& position) override;
    void setupBuffers() override;

protected:
    void generateRandomColors() override;

private:
    static const int numVertices = 24;
    float vertices[numVertices * 9];
    GLushort indices[36] = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
    };
};

#endif // CUBE_H
