#pragma once

#include "Qyhs_pipeline.hpp"
#include "Qyhs_frame_info.hpp"
#include "GameObject.hpp"
#include <memory>
namespace QYHS
{
	
	struct SimplePushConstantData
	{
		glm::mat4 modelMatrix{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(QyhsDevice &device,VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();



		SimpleRenderSystem(const SimpleRenderSystem &) = delete;
		SimpleRenderSystem & operator=(const SimpleRenderSystem&) = delete;

		void renderGameObject(FrameInfo & frameInfo);

	private:

		void createPipeline(VkRenderPass renderPass);
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		

		QyhsDevice &qyhsDevice;

		
		std::unique_ptr<QyhsPipeline> qyhsPipeline;
		VkPipelineLayout pipelineLayout;
	};
}