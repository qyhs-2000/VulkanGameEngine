#include "QyhsWindow.hpp"


namespace QYHS
{
	QyhsWindow::QyhsWindow( int w,  int h, std::string name)
		:width { w }, height{ h }, windowName{ name }
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
		//Initialize the GLFW Library,should call this function before you call most of the glfw function
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}
