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
    
    void update();
    glm::mat4 getViewMatrix();
    
    glm::vec3 pos = glm::vec3(0);
    
    glm::vec3 vel = glm::vec3(0); // Position velocity per frame (local to camera frame)
    
private:
    glm::vec3 lookVector, up;
    glm::mat4 view;
};

#endif /* Camera_h */
