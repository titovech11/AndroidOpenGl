#include "Renderer.h"
#include <android/log.h>
#include <cstdlib>
#include <ctime>

#define LOG_TAG "Renderer"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

Renderer::Renderer(int cubesCount, int pyramidesCount) : program(0), angleX(0.0f), angleY(0.0f) {
    int allCount = cubesCount + pyramidesCount;
    float x_offset = 2.5;
    for (int i = 0; i < cubesCount; ++i) {
        shapes.push_back(new Cube());
    }
    shapes.push_back(new Sphere(1.0f, 36, 18));
    for (int i = 0; i < pyramidesCount; ++i) {
        shapes.push_back(new Pyramid());
    }
    float x_pos = 0 - (allCount / 2 * x_offset), y_pos = 0, z_pos = 0;
    for (Shape* shape : shapes) {
        shape->setPosition({x_pos, y_pos, z_pos});
        x_pos += x_offset;
    }
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

Renderer::~Renderer() {
    if (program) {
        glDeleteProgram(program);
    }
    for (Shape* shape : shapes) {
        delete shape;
    }
}

GLuint Renderer::loadShader(GLenum type, const char *shaderSrc) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSrc, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(infoLen);
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            LOGE("Error compiling shader: %s", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint Renderer::createProgram(const char *vertexSource, const char *fragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) return 0;
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) return 0;

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char *buf = (char *)malloc(bufLength);
                glGetProgramInfoLog(program, bufLength, nullptr, buf);
                LOGE("Could not link program: %s", buf);
                free(buf);
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void Renderer::updateVPMatrix() {
    vpMatrix = projection * view;
}

void Renderer::onSurfaceCreated() {
    static const char *vertexShaderSource = R"(#version 300 es
        layout(location = 0) in vec4 vPosition;
        layout(location = 1) in vec3 vNormal;
        layout(location = 2) in vec3 vColor;
        uniform mat4 uMVPMatrix;
        uniform mat4 uModelMatrix;
        out vec3 fragNormal;
        out vec3 fragPosition;
        out vec3 fragColor;
        void main() {
            fragPosition = vec3(uModelMatrix * vPosition);
            fragNormal = mat3(transpose(inverse(uModelMatrix))) * vNormal;
            fragColor = vColor;
            gl_Position = uMVPMatrix * vPosition;
        }
    )";

    static const char *fragmentShaderSource = R"(#version 300 es
        precision mediump float;
        in vec3 fragNormal;
        in vec3 fragPosition;
        in vec3 fragColor;
        out vec4 fragColorOutput;
        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform vec3 lightColor;
        void main() {
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * lightColor;

            vec3 norm = normalize(fragNormal);
            vec3 lightDir = normalize(lightPos - fragPosition);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            vec3 result = (ambient + diffuse) * fragColor;
            fragColorOutput = vec4(result, 1.0);
        }
    )";

    program = createProgram(vertexShaderSource, fragmentShaderSource);
    if (!program) {
        LOGE("Could not create program.");
        return;
    }

    for (Shape* shape : shapes) {
        shape->setupBuffers();
    }

    glEnable(GL_DEPTH_TEST);

    LOGI("Surface created");

    srand(time(nullptr)); // Инициализация генератора случайных чисел
}

void Renderer::onDrawFrame() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Установка цвета фона
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Обновляем углы вращения
    angleX += 0.2f;
    angleY += 0.2f;

    updateVPMatrix();
    int i = 0;
    for (Shape* shape : shapes) {
        if(i % 2 == 0) {
            shape->setRotation(-angleX, -angleY);
        } else {
            shape->setRotation(angleX, angleY);
        }

        shape->draw(program, vpMatrix);
        ++i;
    }

    LOGI("Frame drawn");
}

void Renderer::onSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    float aspect = (float)width / (float)height;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    updateVPMatrix();
    LOGI("Surface changed: width = %d, height = %d", width, height);
}

void Renderer::handleTouch(float dx, float dy) {
    angleX += dy;
    angleY += dx;
    LOGI("Touch handled: dx = %f, dy = %f", dx, dy);
}
