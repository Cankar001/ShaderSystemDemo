#include "FPSCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define PI 3.1415926535897932f

namespace ShaderSystem
{
	FPSCamera::FPSCamera(CameraProjection inProjection, uint32_t width, uint32_t height)
		: Camera(inProjection, width, height)
	{
		mYaw = 3.0f * PI / 4.0f;
		mPitch = PI / 4.0f;

		UpdateCameraView();
	}

	FPSCamera::~FPSCamera()
	{
	}
	
	void FPSCamera::UpdateControls(float inDeltaTime)
	{
		// TODO: If we want to enable camera controls, we will need the input system as well.

		UpdateCameraView();
	}

	glm::vec3 FPSCamera::GetUpDirection()
	{
		return glm::normalize(glm::cross(GetRightDirection(), GetForwardDirection()));
	}

	glm::vec3 FPSCamera::GetRightDirection()
	{
		return glm::normalize(glm::cross(GetForwardDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	glm::vec3 FPSCamera::GetForwardDirection()
	{
		glm::vec3 front;
		front.x = cosf(glm::radians(mYaw)) * cosf(glm::radians(mPitch));
		front.y = sinf(glm::radians(mPitch));
		front.z = sinf(glm::radians(mYaw)) * cosf(glm::radians(mPitch));
		return glm::normalize(front);
	}

	void FPSCamera::UpdateCameraView()
	{
		mViewMatrix = glm::lookAt(mPosition, mPosition + GetForwardDirection(), GetUpDirection());
	}

	float FPSCamera::MovementSpeed() const
	{
		return 0.01f;
	}

	float FPSCamera::RotationSpeed() const
	{
		return 0.08f;
	}

	float FPSCamera::RunningSpeed() const
	{
		return 0.025f;
	}
}
