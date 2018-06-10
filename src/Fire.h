#pragma once

#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <vector>
#include <memory>
#include "GLSL.h"
#include "glm/glm.hpp"

class Program;

class Fire {

public:
    void init();
    void init_texture(std::string resourceDirectroy);
    void update(double frametime);
    void draw(const std::shared_ptr<Program> prog) const;

private:
    unsigned int textureID = 0;

    GLushort eleBuf[6] = {
        0, 1, 2, 
        2, 3, 0,
    };

    GLfloat posBuf[12] = {
        -1.0, -1.0,  1.0, // Bottom left
         1.0, -1.0,  1.0, // Bottom right
         1.0,  1.0,  1.0, // Top right
        -1.0,  1.0,  1.0, // Top left
    };

    GLfloat texBuf[8] = {
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        0.0, 0.0
    };

    unsigned int eleBufID = 0;
    unsigned int posBufID = 0;
    unsigned int texBufID = 0;
    unsigned int vaoID = 0;

    glm::mat4 M;

    const int framesPerSecond = 30;

    glm::vec2 to, to2;

    double time;
    float t;

    glm::vec2 pos, vel;

    const glm::vec3 scale = glm::vec3(100, 100, 1);
};

#endif
