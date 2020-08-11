#include "Camera.h"
#include "glm/gtx/transform.hpp"

Camera::Camera()
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTargetPos(glm::vec3(0.0f,0.0f,0.0f)),
	mUp(glm::vec3(0.0f,1.0f,0.0f)),
	mPitch(0.0f),
	mYaw(0.0f)
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

OrbitCamera::OrbitCamera()
	:mRadius(10.0f)
{
}

void OrbitCamera::setLookAt(const glm::vec3& target)
{
	mTargetPos=target;
}

void OrbitCamera::setRadius(float radius)
{
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch)
{
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}


// OrbitCamera - Calculates the front vector from the Camera's
void OrbitCamera::updateCameraVectors()
{
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}
