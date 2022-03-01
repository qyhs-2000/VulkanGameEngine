#include "Light_render_system.hpp"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace QYHS
{
	PointLightSystem::PointLightSystem(QyhsDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		:qyhsDevice(device)
	{
		createPipelineLayout(globalSetLayout);
		createPipeline(renderPass);

	}

	PointLightSystem::~PointLightSystem()
	{
		vkDestroyPipelineLayout(qyhsDevice.device(), pipelineLayout, nullptr);
	}

	

	
	void PointLightSystem::createPipeline(VkRenderPass renderPass)
	{

		assert(pipelineLayout != nullptr&&"Cannot create pipelineLayout before swap chain!");

		PipelineConfigInfo pipelineConfig{};
		QyhsPipeline::defaultPipelineConfigInfo(
			pipelineConfig);
		pipelineConfig.bindingDescriptors.clear();
		pipelineConfig.attributeDescriptors.clear();
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		qyhsPipeline = std::make_unique<QyhsPipeline>(
			qyhsDevice,
			"shaders/point_light.vert.spv",
			"shaders/point_light.frag.spv",
			pipelineConfig);
	}



	void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		/*VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.size = sizeof(SimplePushConstantData);
		pushConstantRange.offset = 0;*/


		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(qyhsDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout !");
		}
	}


	void PointLightSystem::render(FrameInfo & frameInfo)
	{
		qyhsPipeline->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo. globalDescriptorSet, 0, nullptr);

		vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
	}
}