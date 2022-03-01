#include "Qyhs_Window.hpp"


namespace QYHS
{
	QyhsWindow::QyhsWindow(int w, int h, std::string name)
		:width(w),height(h),windowName(name)
	{
		InitWindow();
	}




	QyhsWindow::~QyhsWindow()
	{
		
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void QyhsWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void QyhsWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR * surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}
	void QyhsWindow::framebufferResizeCallback(GLFWwindow * window, int width, int height)
	{
		std::cout<< "Move!" << std::endl;
		auto qyhsWindow = reinterpret_cast<QyhsWindow *>(glfwGetWindowUserPointer(window));
		qyhsWindow->framebufferResized = true;
		qyhsWindow->width = width;
		qyhsWindow->height = height;
	}
}