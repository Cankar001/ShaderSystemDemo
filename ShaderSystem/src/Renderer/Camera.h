#pragma once

#include "Core/Core.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"

#include <glm/glm.hpp>

namespace ShaderSystem
{
	enum class CameraProjection
	{
		Orthographic = 0,
		Perspective,
	};

	class Camera
	{
	public:

		Camera(CameraProjection inProjection, uint32_t width, uint32_t height);
		virtual ~Camera();

		virtual void UpdateControls(float inDeltaTime) = 0;

		bool OnWindowResize(WindowResizeEvent &e);
		void OnEvent(Event& e);

		const glm::mat4& GetProjection() const { return mProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return mViewMatrix; }

		const glm::vec3& GetPosition()	 const { return mPosition; }
		const glm::vec3 GetYawPitchRoll()  const { return glm::vec3(mYaw, mPitch, mRoll); }

		const float GetYaw()   const { return mYaw; }
		const float GetPitch() const { return mPitch; }
		const float GetRoll()  const { return mRoll; }

		void SetPosition(const glm::vec3& inPosition) { mPosition = inPosition; }
		void SetYawPitchRoll(const glm::vec3& inOrientation);

		void SetProjectionMatrix(const glm::mat4& inProjectionMatrix) { mProjectionMatrix = inProjectionMatrix; }
		void SetViewMatrix(const glm::mat4& inViewMatrix) { mViewMatrix = inViewMatrix; }

		void SetYaw(float inYaw) { mYaw = inYaw; }
		void SetPitch(float inPitch) { mPitch = inPitch; }
		void SetRoll(float inRoll) { mRoll = inRoll; }

		void InvertPitch();

		float GetExposure() const { return mExposure; }
		float& GetExposure() { return mExposure; }

		void SetPerspective(float verticalFOV = 45.0f, float nearClip = 0.01f, float farClip = 10000.0f);
		void SetOrthographic(float width, float height, float size, float nearClip = -1.0f, float farClip = 1.0f);
		void SetOrthographic(float size = 10.0f, float nearClip = -1.0f, float farClip = 1.0f);
		void SetViewportSize(uint32_t width, uint32_t height);

		inline void SetPerspectiveFOV(float inFov) { mPerspectiveFOV = glm::radians(inFov); }
		inline void SetPerspectiveNearPlane(float inNearPlane) { mPerspectiveNear = inNearPlane; }
		inline void SetPerspectiveFarPlane(float inFarPlane) { mPerspecticeFar = inFarPlane; }

		inline void SetOrthographicSize(float inSize) { mOrthographicSize = inSize; }
		inline void SetOrthographicNearPlane(float inNearPlane) { mOrthographicNear = inNearPlane; }
		inline void SetOrthographicFarPlane(float inFarPlane) { mOrthographicFar = inFarPlane; }

		inline float GetPerspectiveFOV() const { return glm::degrees(mPerspectiveFOV); }
		inline float GetPerspectiveNearPlane() const { return mPerspectiveNear; }
		inline float GetPerspectiveFarPlane() const { return mPerspecticeFar; }

		inline float GetOrthographicSize() const { return mOrthographicSize; }
		inline float GetOrthographicNearPlane() const { return mOrthographicNear; }
		inline float GetOrthographicFarPlane() const { return mOrthographicFar; }

		virtual void GetDirectionVectors(glm::vec3& front, glm::vec3& up, glm::vec3& left);

		void SetProjectionType(CameraProjection inType) { mProjectionType = inType; }
		CameraProjection GetCurrentProjectionType() const { return mProjectionType; }
		bool IsPerspective() const { return mProjectionType == CameraProjection::Perspective; }
		bool IsOrthographic() const { return mProjectionType == CameraProjection::Orthographic; }

	private:

		void SetProjectionInternal(uint32_t width, uint32_t height);

	protected:

		CameraProjection mProjectionType = CameraProjection::Perspective;

		float mPerspectiveFOV = glm::radians(45.0f);
		float mPerspectiveNear = 0.01f, mPerspecticeFar = 10000.0f;

		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f, mOrthographicFar = 1.0f;

		glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 mViewMatrix = glm::mat4(1.0f);
		glm::vec3 mPosition = glm::vec3(0, 0, 0);
		float mYaw = 0.0f;
		float mPitch = 0.0f;
		float mRoll = 0.0f;
		float mExposure = 1.0f;
		uint32_t mViewportWidth = 0, mViewportHeight = 0;
	};
}