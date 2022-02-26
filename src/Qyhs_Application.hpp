#pragma once
#include "Qyhs_Window.hpp"
#include "Qyhs_pipeline.hpp"
#include "Qyhs_Camera.hpp"
#include "Camera_movement_controller.hpp"
#include "Qyhs_RenderSystem.hpp"
#include "GameObject.hpp"
#include "Qyhs_buffer.hpp"
#include "Qyhs_Descriptors.hpp"
#include <memory>
namespace QYHS
{
	

	class QyhsApplication
	{
	public:
		QyhsApplication();
		~QyhsApplication();

		static constexpr int WIDTH = 1200;
		static constexpr int HEIGHT = 900;

		void run();

		QyhsApplication(const QyhsApplication &) = delete;
		QyhsApplication & operator=(const QyhsApplication&) = delete;

		
	
	private:
		void loadGameObjects();

		QyhsWindow qyhsWindow{ WIDTH, HEIGHT,"Hello Vulkan" };
		QyhsDevice qyhsDevice{ qyhsWindow };
	
		QyhsRenderer qyhsRender{ qyhsWindow,qyhsDevice };

		std::unique_ptr<QyhsDescriptorPool> globalPool{};
		QyhsGameObject::Map gameObjects;
	};
}