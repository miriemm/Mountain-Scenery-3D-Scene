#include "Camera.hpp"

#include <GLFW/glfw3.h>

namespace gps {

    // constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        // front
        this->cameraDirection = glm::normalize(cameraTarget - cameraPosition); 
       // right
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, this->cameraUpDirection));
       // presentation mode is initially set to false
        this->presentationMode = false;
    }

    // returnez the view matrix
    glm::mat4 Camera::getViewMatrix() {

        // in cazul in care ma aflu in presentation mode
        if (presentationMode) {
            float radius = 50.0f;
            float camX = sin(glfwGetTime()) * radius;
            float camZ = cos(glfwGetTime()) * radius;
            return glm::lookAt(glm::vec3(camX, 20.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else {
            return glm::lookAt(cameraPosition, cameraPosition + cameraDirection, this->cameraUpDirection);
        }
    }

    glm::vec3 Camera::getCameraTarget()
    {
        return cameraTarget;
    }

    // actualizez parametrii interni ai camerei in urma unui eveniment de miscare a camerei
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction) {
        case MOVE_FORWARD:
            cameraPosition += cameraDirection * speed;
            break;
        case MOVE_BACKWARD:
            cameraPosition -= cameraDirection * speed;
            break;
        case MOVE_RIGHT:
            cameraPosition += cameraRightDirection * speed;
            break;
        case MOVE_LEFT:
            cameraPosition -= cameraRightDirection * speed;
            break;
        }
    }

    // actualizez parametrii interni ai camerei, dupa ce rotim camera
    // yaw reprezinta rotatia camerei in jurul axei y
    // iar pitch reprezinta rotatia camerei in jurul axei x

    void Camera::rotate(float pitch, float yaw) {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraDirection = glm::normalize(direction);

    }

    // incepem presentation mode-ul
    // variabila este setata pe true
    void Camera::startPresentationMode() {
        presentationMode = true;
    }

    // oprim presentation mode-ul
    // variabila este setata pe false
    void Camera::stopPresentationMode() {
        presentationMode = false;
    }

    // verificam daca este presentation mode
    bool Camera::isPresentationMode() {
        return presentationMode;
    }
}