#pragma once

#include "Camera.h"

namespace ShaderSystem
{
	class FPSCamera : public Camera
	{
	public:

		FPSCamera(CameraProjection inProjection, uint32_t width, uint32_t height);
		virtual ~FPSCamera();

		virtual void UpdateControls(float inDeltaTime) override;

		glm::vec3 GetUpDirection();
		glm::vec3 GetRightDirection();
		glm::vec3 GetForwardDirection();

	private:

		void UpdateCameraView();
		float MovementSpeed() const;
		float RotationSpeed() const;
		float RunningSpeed() const;
	};
}
