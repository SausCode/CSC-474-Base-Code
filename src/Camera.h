#pragma  once
#ifndef Camera_h
#define Camera_h

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"

class Camera {
public:
    Camera();
    ~Camera() {}
    
    void update(glm::vec2 playerPos, glm::vec2 playerVel, int screenWidth);
    glm::mat4 getViewMatrix();
    glm::mat4 getFlatViewMatrix();
    
    glm::vec2 pos;
    
    glm::vec2 vel; // Position velocity per frame (local to camera frame)
    
private:
    glm::vec3 lookVector, up;
    glm::mat4 view;
};

#endif /* Camera_h */
