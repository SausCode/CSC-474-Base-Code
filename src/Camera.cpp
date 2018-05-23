#include <stdio.h>
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() {
    glm::mat4 view;

    up = glm::vec3(0, 1, 0);

    pos = glm::vec2(0);
    vel = glm::vec2(0);
}

void Camera::update() {

    pos.x += vel.x;
    pos.y += vel.y;

    view = glm::translate(view, glm::vec3(vel, 0.f));
}

glm::mat4 Camera::getViewMatrix() {
	glm::vec3 eye = glm::vec3(0, 0, 50);
	glm::vec3 center = glm::vec3(-5, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
    return glm::lookAt(eye, center, up);
}

glm::mat4 Camera::getFlatViewMatrix() {
	return view;
}
