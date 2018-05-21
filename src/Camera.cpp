#include <stdio.h>
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() {
    glm::mat4 view;

    up = glm::vec3(0, 1, 0);

    //view = glm::rotate(view, glm::radians(45.f), up);

    //view = glm::inverse(view);
}

void Camera::update() {

    pos.x += vel.x;
    pos.y += vel.y;

    view = glm::translate(view, pos);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::rotate(view, glm::radians(-10.f), glm::vec3(0, 1, 0));
}
