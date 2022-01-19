#include "Application.hpp"
namespace QYHS
{
	void Application::run()
	{
		while (!qyhsWindow.shouldCloseWindow()) {
			glfwPollEvents();
		}
	}
}