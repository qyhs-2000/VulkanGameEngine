#pragma once

#include "QyhsWindow.hpp"
#include "QyhsPipeline.hpp"



namespace QYHS
{
	class Application
	{
	public:
		static constexpr int WIDTH = 1000;
		static constexpr int HEIGHT = 800;

		
		void run() ;
	private:
		QyhsWindow qyhsWindow{ WIDTH,HEIGHT,"Hello Vulkan!" };
		QyhsPipeline pipeline{ "Shader/simple_shader.vert.spv","Shader/simple_shader.frag.spv" };

	};
}