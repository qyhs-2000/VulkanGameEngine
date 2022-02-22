#pragma once
#include "Qyhs_Window.hpp"
#include "Qyhs_pipeline.hpp"
#include "Qyhs_Camera.hpp"
#include "Camera_movement_controller.hpp"
#include "Qyhs_RenderSystem.hpp"
#include "GameObject.hpp"
#include <memory>
namespace QYHS
{
	/*struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};*/


	class QyhsApplication
	{
	public:
		QyhsApplication();
		~QyhsApplication();

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

		QyhsApplication(const QyhsApplication &) = delete;
		QyhsApplication & operator=(const QyhsApplication&) = delete;

		
	
	private:
		void loadGameObjects();

		QyhsWindow qyhsWindow{ WIDTH, HEIGHT,"Hello Vulkan" };
		QyhsDevice qyhsDevice{ qyhsWindow };
	
		QyhsRenderer qyhsRender{ qyhsWindow,qyhsDevice };

		std::vector<QyhsGameObject> gameObjects;
	};
}