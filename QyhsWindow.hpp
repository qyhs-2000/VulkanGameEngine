#pragma once 
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
namespace QYHS
{
	class QyhsWindow
	{

	public:
		QyhsWindow( int w,  int h, std::string name);
		~QyhsWindow();
		bool shouldCloseWindow() { return glfwWindowShouldClose(window); }


		QyhsWindow(const QyhsWindow&) = delete;
		QyhsWindow& operator =(const QyhsWindow&) = delete;

	private:
		void InitWindow();
		const int width;
		const int height;
		std::string windowName;
		GLFWwindow* window;


	};
}
