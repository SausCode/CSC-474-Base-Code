#pragma once

#ifndef WATERBAR_H
#define WATERBAR_H

#include <vector>
#include <memory>
#include "GLSL.h"
#include "glm/glm.hpp"

class Program;

class Waterbar {

public:
    Waterbar(float width);

    void init();
    void update(float water);
    void draw(const std::shared_ptr<Program> prog) const;

private:

    GLushort eleBuf[6] = {
        0, 1, 2, 
        2, 3, 0,
    };

    GLfloat posBuf[12] = {
        50.0, 15.0, 1.0, // Bottom left
        50.0, 15.0, 1.0, // Bottom right
        50.0, 40.0, 1.0, // Top right
        50.0, 40.0, 1.0, // Top left
    };

    GLfloat waterBuf[4] = {
        0.0, // Bottom left
        1.0, // Bottom right
        1.0, // Top right
        0.0, // Top left
    };

    unsigned int eleBufID = 0;
    unsigned int posBufID = 0;
    unsigned int waterBufID = 0;
    unsigned int vaoID = 0;

    float width;
    float water;

    glm::vec3 color;
};

#endif