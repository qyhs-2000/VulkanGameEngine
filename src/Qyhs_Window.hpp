#pragma once

#define GLFW_INCLUDE_VULKAN
#include <string>
#include <GLFW/glfw3.h>

#include <iostream>
namespace QYHS
{
	class QyhsWindow
	{
	public:
		QyhsWindow(int w,int h,std::string name);
		~QyhsWindow();

		//init window
		void InitWindow();

		bool bShouldClose() { return glfwWindowShouldClose(window); }

		QyhsWindow & operator=(const QyhsWindow& ) = delete;
		QyhsWindow(const QyhsWindow &) = delete;

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
			
		GLFWwindow * getGlfwWindow() { return window; }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR * surface);
		bool wasWindowResizedFlag() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }

	private:
		static void framebufferResizeCallback(GLFWwindow * window, int width, int height);
		int width;
		int height;
		bool framebufferResized = false;


		const std::string windowName;

		GLFWwindow * window;
	
	};
}