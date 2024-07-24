#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include <vector>

class Sphere : public Shape {
public:
    Sphere(float radius, int sectorCount, int stackCount);
    ~Sphere() override;
    void draw(GLuint program, const glm::mat4& vpMatrix) override;
    void updateColors() override;
    void setRotation(float angleX, float angleY) override;
    void setPosition(const glm::vec3& position) override;
    void setupBuffers() override;

protected:
    void generateRandomColors() override;

private:
    void buildVertices();

    float radius;
    int sectorCount;
    int stackCount;
    std::vector<float> vertices;
    std::vector<GLushort> indices;
};

#endif // SPHERE_H
