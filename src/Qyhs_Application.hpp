#pragma once
#include "Qyhs_Descriptors.hpp"
#include "Qyhs_Device.hpp"
#include "GameObject.hpp"
#include "Qyhs_renderer.hpp"
#include "Qyhs_Window.hpp"




// std
#include <memory>
#include <vector>
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

		QyhsWindow qyhsWindow{ WIDTH, HEIGHT,"Qyhs Engine" };
		QyhsDevice qyhsDevice{ qyhsWindow };
	
		QyhsRenderer qyhsRender{ qyhsWindow,qyhsDevice };

		std::unique_ptr<QyhsDescriptorPool> globalPool{};
		QyhsGameObject::Map gameObjects;
	};
}