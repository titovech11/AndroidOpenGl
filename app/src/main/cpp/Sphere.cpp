#include "Sphere.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <android/log.h>

#define LOG_TAG "Sphere"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

Sphere::Sphere(float radius, int sectorCount, int stackCount)
        : radius(radius), sectorCount(sectorCount), stackCount(stackCount) {
    rotationX = 0.0f;
    rotationY = 0.0f;
    pos = {0.0f, 0.0f, 0.0f};
    buildVertices();
    updateColors();
}

Sphere::~Sphere() {
    if (vao) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo) {
        glDeleteBuffers(1, &ebo);
    }
}

void Sphere::buildVertices() {
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        for(int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(x * lengthInv);
            vertices.push_back(y * lengthInv);
            vertices.push_back(z * lengthInv);
            vertices.push_back(1.0f);  // Placeholder for color
            vertices.push_back(1.0f);  // Placeholder for color
            vertices.push_back(1.0f);  // Placeholder for color
        }
    }

    for(int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);     // beginning of current stack
        int k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if(i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::setupBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    LOGI("Generated buffers: vao = %d, vbo = %d, ebo = %d", vao, vbo, ebo);

    if (vao == 0 || vbo == 0 || ebo == 0) {
        LOGE("Failed to generate buffers: vao = %d, vbo = %d, ebo = %d", vao, vbo, ebo);
        return;
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    LOGI("Buffers setup completed");
}

void Sphere::generateRandomColors() {
    for (size_t i = 0; i < vertices.size(); i += 9) {
        vertices[i + 6] = static_cast<float>(rand()) / RAND_MAX;
        vertices[i + 7] = static_cast<float>(rand()) / RAND_MAX;
        vertices[i + 8] = static_cast<float>(rand()) / RAND_MAX;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
}

void Sphere::updateColors() {
    generateRandomColors();
}

void Sphere::setRotation(float angleX, float angleY) {
    rotationX = angleX;
    rotationY = angleY;
    modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Sphere::setPosition(const glm::vec3& position) {
    pos = position;
}

void Sphere::draw(GLuint program, const glm::mat4& vpMatrix) {
    glUseProgram(program);
    glm::mat4 mvpMatrix = vpMatrix * modelMatrix * glm::translate(glm::mat4(1.0f), pos);
    glUniformMatrix4fv(glGetUniformLocation(program, "uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(glGetUniformLocation(program, "lightPos"), 3.0f, 3.0f, 3.0f);
    glUniform3f(glGetUniformLocation(program, "viewPos"), 0.0f, 0.0f, 5.0f);
    glUniform3f(glGetUniformLocation(program, "lightColor"), 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
