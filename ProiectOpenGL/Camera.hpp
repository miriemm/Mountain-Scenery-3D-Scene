#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <string>

namespace gps {

    enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT };

    class Camera
    {
    public:

        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
        glm::mat4 getViewMatrix();
        glm::vec3 getCameraTarget();
        void move(MOVE_DIRECTION direction, float speed);
        void rotate(float pitch, float yaw);
        void startPresentationMode();
        void stopPresentationMode();
        bool isPresentationMode();
    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
        
        bool presentationMode;
    };

}

#endif /* Camera_hpp */
