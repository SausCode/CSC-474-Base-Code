#include <stdio.h>
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() {
    glm::mat4 view;

    up = glm::vec3(0, 1, 0);

    pos = glm::vec2(0);
    vel = glm::vec2(0);
}

void Camera::update(glm::vec2 playerPos, glm::vec2 playerVel, int screenWidth) {
    if (playerPos.x + pos.x > 0.7f * screenWidth && playerVel.x > 0.f) {
        pos.x -= playerVel.x;
    }
    else if (playerPos.x + pos.x < 0.3f * screenWidth && playerVel.x < 0.f) {
        pos.x -= playerVel.x;
    }

    if (pos.x >= 0.f) {
        pos.x = 0.f;
    }
}

glm::mat4 Camera::getViewMatrix() {
	glm::vec3 eye = glm::vec3(0, 0, 50);
	glm::vec3 center = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
    glm::mat4 look =  glm::lookAt(eye, center, up);

    return glm::translate(look, glm::vec3(pos.x, 0, 0));
}

glm::mat4 Camera::getFlatViewMatrix() {
	return view;
}
