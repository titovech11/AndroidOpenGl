#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Sphere.h"

class Renderer {
public:
    Renderer(int cubesCount, int pyramidesCount);
    ~Renderer();
    void onSurfaceCreated();
    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void handleTouch(float dx, float dy);

private:
    GLuint program;
    std::vector<Shape*> shapes;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 vpMatrix;
    float angleX;
    float angleY;

    GLuint loadShader(GLenum type, const char *shaderSrc);
    GLuint createProgram(const char *vertexSource, const char *fragmentSource);
    void updateVPMatrix();
};

#endif // RENDERER_H
