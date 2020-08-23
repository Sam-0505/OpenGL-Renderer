#include "Camera.h"
#include "glm/gtx/transform.hpp"

// Default camera values
const float DEF_FOV = 45.0f; // degrees

Camera::Camera()
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTargetPos(glm::vec3(0.0f,0.0f,0.0f)),
	mUp(glm::vec3(0.0f,1.0f,0.0f)),
	mRight(0.0f, 0.0f, 0.0f),
	WORLD_UP(0.0f, 1.0f, 0.0f),
	mPitch(0.0f),
	mYaw(glm::pi<float>()),
	mFOV(DEF_FOV)
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

//------------------------------------------------------------
// Base Camera - Returns camera's local look vector
//------------------------------------------------------------
const glm::vec3& Camera::getLook() const
{
	return mLook;
}

//------------------------------------------------------------
// Base Camera - Returns camera's local right vector
//------------------------------------------------------------
const glm::vec3& Camera::getRight() const
{
	return mRight;
}

//------------------------------------------------------------
// Base Camera - Returns camera's local up vector
//------------------------------------------------------------
const glm::vec3& Camera::getUp() const
{
	return mUp;
}

const glm::vec3& Camera::getViewPos()
{
	return mPosition;
}

FPSCamera::FPSCamera(glm::vec3 position , float yaw, float pitch)
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

void FPSCamera::setPosition(const glm::vec3& position)
{
	mPosition = position;
}

void FPSCamera::rotate(float yaw, float pitch)
{
	mYaw += glm::radians(yaw);
	mPitch +=glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}

void FPSCamera::move(const glm::vec3& offsetPos)
{
	mPosition += offsetPos;
	updateCameraVectors();
}

void FPSCamera::updateCameraVectors()
{
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	// Re-calculate the Right and Up vector.  For simplicity the Right vector will
// be assumed horizontal w.r.t. the world's Up vector.
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
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
