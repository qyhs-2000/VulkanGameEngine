#pragma once
#include "Qyhs_Window.hpp"
#include "Qyhs_pipeline.hpp"
#include "Qyhs_Renderer.hpp"
#include "Qyhs_frame_info.hpp"
#include "GameObject.hpp"
#include <memory>
namespace QYHS
{
	
	class PointLightSystem
	{
	public:
		PointLightSystem(QyhsDevice &device,VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();



		PointLightSystem(const PointLightSystem &) = delete;
		PointLightSystem & operator=(const PointLightSystem&) = delete;

		void render(FrameInfo & frameInfo);

	private:

		void createPipeline(VkRenderPass renderPass);
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		

		QyhsDevice &qyhsDevice;

		
		std::unique_ptr<QyhsPipeline> qyhsPipeline;
		VkPipelineLayout pipelineLayout;
	};
}