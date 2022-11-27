#ifndef PBR_CAMERA_H
#define PBR_CAMERA_H

#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <GLES3/gl3.h>

namespace {
	// Default camera values
	const float SPEED =0.05f;
	const float DISTANCE = 2.5f;
	const float SENSITIVITY = 0.05f;
	const float ZOOM = 45.0f;
}

class PbrCamera
{
public:
	glm::vec3 mPosition;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;
	glm::vec3 mTarget;

	float mMovementSpeed;
	float mMouseSensitivity;
	float mZoom;

	bool mDragging = true;
	bool mPanning = false;

	PbrCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.3f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f));
	PbrCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float targetX, float targetY, float targetZ);


	glm::mat4 getViewMatrix();
	void processMouseMovement(float xoffset, float yoffset);
	void processMousePress(bool leftPressed, bool rightPressed);
	void processMouseScroll(float yoffset);

private:
	void updateCameraVectors();
};

#endif //PBR_CAMERA_H
