#pragma once

#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <vector>
#include <memory>
#include "GLSL.h"
#include "glm/glm.hpp"

#include "Water.h"
#include "Player.h"

class Program;

class Fire {

public:
    Fire(float x, float y);

    void init();
    void init_texture();
    void update(double frametime, std::vector<Water> &waterDroplets);
    void checkWaterDropletsCollision(std::vector<Water> &waterDroplets);
    void checkPlayerDamage(Player* player);
    void draw(const std::shared_ptr<Program> prog) const;

    float size = 100;

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

    int framesPerSecond = 30;

    glm::vec2 to, to2;

    double time;
    float t;

    glm::vec2 pos, vel;


};

#endif
