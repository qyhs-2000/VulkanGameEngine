#pragma once

#include "GameObject.hpp"
#include "Qyhs_Window.hpp"
namespace QYHS
{
	class CameraMovementController
	{
	public:
		struct KeyMappings
		{
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		
		
		void moveInPlaneXZ(GLFWwindow * window, float deltaTime, QyhsGameObject& gameObject);

		float moveSpeed{ 3.f };
		float lookSpeed{ 1.5f };
		KeyMappings keys{};
		

		
	};

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	
}

