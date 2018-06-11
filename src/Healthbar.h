#pragma once

#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <string>
#include <vector>
#include <memory>
#include "GLSL.h"
#include "glm/glm.hpp"

class Program;

class Healthbar {

public:
    Healthbar(float width);

    void init();
    void update(float health);
    void draw(const std::shared_ptr<Program> prog) const;

private:

    GLushort eleBuf[6] = {
        0, 1, 2, 
        2, 3, 0,
    };

    GLfloat posBuf[12] = {
        50.0, 50.0, 1.0, // Bottom left
        50.0, 50.0, 1.0, // Bottom right
        50.0, 75.0, 1.0, // Top right
        50.0, 75.0, 1.0, // Top left
    };

    GLfloat healthBuf[4] = {
        0.0, // Bottom left
        1.0, // Bottom right
        1.0, // Top right
        0.0, // Top left
    };

    unsigned int eleBufID = 0;
    unsigned int posBufID = 0;
    unsigned int healthBufID = 0;
    unsigned int vaoID = 0;

    float width;
    float health;
};

#endif
