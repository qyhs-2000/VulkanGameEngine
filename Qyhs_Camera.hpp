#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

namespace QYHS
{
	class QyhsCamera
	{
	public:
		void setOrthographicProjection(float left,float right,float buttom,float top,float near,float far);
		void setPerspectiveProjection(float fovy, float aspect, float near, float far);
		void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.f,-1.f,0.f });
		void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f,-1.f,0.f });
		void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

		const glm::mat4 & getProjectionMatrix() { return projectionMatrix; }
		const glm::mat4 & getViewMatrix() { return viewMatrix; }



	private:
		glm::mat4 projectionMatrix{ 1.f };
		glm::mat4 viewMatrix{ 1.0f };//camera transform
	};
}
