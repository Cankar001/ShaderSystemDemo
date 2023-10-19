#include "Camera.h"

#include "Core/Logger.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ShaderSystem
{
	Camera::Camera(CameraProjection inProjection, uint32_t width, uint32_t height)
		: mProjectionType(inProjection), mViewportWidth(width), mViewportHeight(height)
	{
		SetProjectionInternal(width, height);
	}
	
	Camera::~Camera()
	{
	}

	bool Camera::OnWindowResize(WindowResizeEvent& e)
	{
		SHADER_SYSTEM_INFO("Camera received window resize event.");
		SetViewportSize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(SHADER_SYSTEM_BIND_CLASS_EVENT_FN(Camera::OnWindowResize));
	}

	void Camera::SetYawPitchRoll(const glm::vec3& inOrientation)
	{
		mYaw = inOrientation.x;
		mPitch = inOrientation.y;
		mRoll = inOrientation.z;
	}
	
	void Camera::InvertPitch()
	{
		mPitch = -mPitch;
	}

	void Camera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		mProjectionType = CameraProjection::Perspective;
		mPerspectiveFOV = glm::radians(verticalFOV);
		mPerspectiveNear = nearClip;
		mPerspecticeFar = farClip;
		SetProjectionInternal(mViewportWidth, mViewportHeight);
	}

	void Camera::SetOrthographic(float width, float height, float size, float nearClip, float farClip)
	{
		mProjectionType = CameraProjection::Orthographic;
		mOrthographicSize = size;
		mOrthographicNear = nearClip;
		mOrthographicFar = farClip;
		SetProjectionInternal((uint32_t)width, (uint32_t)height);
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		mProjectionType = CameraProjection::Orthographic;
		mOrthographicSize = size;
		mOrthographicNear = nearClip;
		mOrthographicFar = farClip;
		SetProjectionInternal(mViewportWidth, mViewportHeight);
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		mViewportWidth = width;
		mViewportHeight = height;
		SetProjectionInternal(width, height);
	}
	
	void Camera::GetDirectionVectors(glm::vec3& front, glm::vec3& up, glm::vec3& left)
	{
		float sx, sy, sz, cx, cy, cz, theta;

		// rotation angle about X-axis (pitch)
		theta = glm::radians(mPitch);
		sx = sinf(theta);
		cx = cosf(theta);

		// rotation angle about Y-axis (yaw)
		theta = glm::radians(mYaw);
		sy = sinf(theta);
		cy = cosf(theta);

		// rotation angle about Z-axis (roll)
		theta = glm::radians(mRoll);
		sz = sinf(theta);
		cz = cosf(theta);

		left.x = cy * cz;
		left.y = sx * sy * cz + cx * sz;
		left.z = -cx * sy * cz + sx * sz;

		up.x = -cy * sz;
		up.y = -sx * sy * sz + cx * cz;
		up.z = cx * sy * sz + sx * cz;

		front.x = sy;
		front.y = -sx * cy;
		front.z = cx * cy;
	}

	void Camera::SetProjectionInternal(uint32_t width, uint32_t height)
	{
		switch (mProjectionType)
		{
		case CameraProjection::Perspective:
		{
			mProjectionMatrix = glm::perspectiveFov(mPerspectiveFOV, (float)width, (float)height, mPerspectiveNear, mPerspecticeFar);
			break;
		}

		case CameraProjection::Orthographic:
		{
			float aspect = (float)width / (float)height;
			float w = mOrthographicSize * aspect;
			float h = mOrthographicSize;
			mProjectionMatrix = glm::ortho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f);
			break;
		}
		}
	}
}
