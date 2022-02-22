#include "Camera_movement_controller.hpp"
#include <iostream>

void QYHS::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << xpos << "   " << ypos << std::endl;
}

QYHS::CameraMovementController::CameraMovementController(QyhsWindow & window)
{
	auto glfwWindow = window.getGlfwWindow();
	glfwSetCursorPosCallback(glfwWindow, QYHS::mouse_callback);
}

void QYHS::CameraMovementController::moveInPlaneXZ(GLFWwindow * window, float deltaTime, QyhsGameObject & gameObject)
{
	glm::vec3 rotate{ 0 };
	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)rotate.y -= 1.f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)rotate.x += 1.f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)rotate.x -= 1.f;




	
	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
	{
		gameObject.transform.rotation += lookSpeed * deltaTime*glm::normalize(rotate);
	}
	gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
	gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

	float yaw = gameObject.transform.rotation.y;
	const glm::vec3 forwardDir{ sin(yaw),0.f,cos(yaw) };
	const glm::vec3 rightDir{ forwardDir.z,0.f,-forwardDir.x };
	const glm::vec3 upDir{ 0.f,-1.f,0.f };

	glm::vec3 movDir{ 0.0f };

	if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) movDir += forwardDir;
	if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) movDir -= forwardDir;
	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) movDir += rightDir;
	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) movDir -= rightDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) movDir += upDir;
	if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) movDir -= upDir;
	if (glm::dot(movDir, movDir) > std::numeric_limits<float>::epsilon())
	{
		gameObject.transform.translation += moveSpeed * deltaTime*glm::normalize(movDir);
	}

	
}


