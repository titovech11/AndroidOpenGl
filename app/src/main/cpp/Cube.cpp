#include "Cube.h"
#include <cstdlib>
#include <ctime>
#include <android/log.h>

#define LOG_TAG "Cube"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

Cube::Cube() {
    rotationX = 0.0f;
    rotationY = 0.0f;
    pos = {0.0f, 0.0f, 0.0f};
    updateColors();
}

Cube::~Cube() {
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

void Cube::setupBuffers() {
    float baseVertices[] = {
            // Позиции
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  // Красная грань
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  // Красная грань
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  // Красная грань
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  // Красная грань

            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  // Зеленая грань
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  // Зеленая грань
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  // Зеленая грань
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  // Зеленая грань

            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  // Синяя грань
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  // Синяя грань
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  // Синяя грань
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  // Синяя грань

            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  // Желтая грань
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  // Желтая грань
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  // Желтая грань
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  // Желтая грань

            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  // Бирюзовая грань
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  // Бирюзовая грань
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  // Бирюзовая грань
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  // Бирюзовая грань

            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // Фиолетовая грань
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // Фиолетовая грань
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  // Фиолетовая грань
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f   // Фиолетовая грань
    };

    for (int i = 0; i < numVertices; ++i) {
        vertices[i * 9 + 0] = baseVertices[i * 6 + 0];
        vertices[i * 9 + 1] = baseVertices[i * 6 + 1];
        vertices[i * 9 + 2] = baseVertices[i * 6 + 2];
        vertices[i * 9 + 3] = baseVertices[i * 6 + 3];
        vertices[i * 9 + 4] = baseVertices[i * 6 + 4];
        vertices[i * 9 + 5] = baseVertices[i * 6 + 5];
        // Цвета будут генерироваться случайным образом
    }

    generateRandomColors();

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    LOGI("Buffers setup completed");
}

void Cube::generateRandomColors() {
    for (int i = 0; i < numVertices; ++i) {
        vertices[i * 9 + 6] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        vertices[i * 9 + 7] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        vertices[i * 9 + 8] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
}

void Cube::updateColors() {
    generateRandomColors();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLint bufferBinding = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bufferBinding);
    if (bufferBinding == vbo) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    } else {
        LOGE("Buffer binding failed: expected %d but got %d", vbo, bufferBinding);
    }
}

void Cube::setRotation(float angleX, float angleY) {
    rotationX = angleX;
    rotationY = angleY;
    modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Cube::setPosition(const glm::vec3& position) {
    pos = position;
}

void Cube::draw(GLuint program, const glm::mat4& vpMatrix) {
    glUseProgram(program);
    glm::mat4 mvpMatrix = vpMatrix * modelMatrix * glm::translate(glm::mat4(1.0f), pos);
    glUniformMatrix4fv(glGetUniformLocation(program, "uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(glGetUniformLocation(program, "lightPos"), 3.0f, 3.0f, 3.0f);
    glUniform3f(glGetUniformLocation(program, "viewPos"), 0.0f, 0.0f, 5.0f);
    glUniform3f(glGetUniformLocation(program, "lightColor"), 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
