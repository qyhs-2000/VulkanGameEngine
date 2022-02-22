#pragma once
#include "Qyhs_Window.hpp"
#include "Qyhs_pipeline.hpp"
#include "Qyhs_Renderer.hpp"
#include "Qyhs_Camera.hpp"
#include "GameObject.hpp"
#include <memory>
namespace QYHS
{
	
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(QyhsDevice &device,VkRenderPass renderPass);
		~SimpleRenderSystem();



		SimpleRenderSystem(const SimpleRenderSystem &) = delete;
		SimpleRenderSystem & operator=(const SimpleRenderSystem&) = delete;

		void renderGameObject(VkCommandBuffer commandBuffer, std::vector<QyhsGameObject>& gameObjects, QyhsCamera & camera);

	private:

		void createPipeline(VkRenderPass renderPass);
		void createPipelineLayout();
		

		QyhsDevice &qyhsDevice;

		
		std::unique_ptr<QyhsPipeline> qyhsPipeline;
		VkPipelineLayout pipelineLayout;
	};
}