#include "Qyhs_RenderSystem.hpp"


#include "Qyhs_Application.hpp"
#include <glm/gtc/constants.hpp>
#include <memory>
#include <array>
#include <iostream>

namespace QYHS
{
	SimpleRenderSystem::SimpleRenderSystem(QyhsDevice &device, VkRenderPass renderPass)
		:qyhsDevice(device)
	{
		createPipelineLayout();
		createPipeline(renderPass);

	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(qyhsDevice.device(), pipelineLayout, nullptr);
	}

	

	/*����pipeline*/
	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{

		assert(pipelineLayout != nullptr&&"Cannot create pipelineLayout before swap chain!");

		PipelineConfigInfo pipelineConfig{};
		QyhsPipeline::defaultPipelineConfigInfo(
			pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		qyhsPipeline = std::make_unique<QyhsPipeline>(
			qyhsDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}






	/*����PipelineLayout*/
	void SimpleRenderSystem::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.size = sizeof(SimplePushConstantData);
		pushConstantRange.offset = 0;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(qyhsDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout !");
		}
	}


	void SimpleRenderSystem::renderGameObject(VkCommandBuffer commandBuffer,std::vector<QyhsGameObject> &gameObjects,QyhsCamera & camera)
	{
		qyhsPipeline->bind(commandBuffer);

		auto projectionView = camera.getProjectionMatrix()*camera.getViewMatrix();
		for (auto &obj : gameObjects)
		{
			
			SimplePushConstantData push{};
			auto modelMatrix = obj.transform.mat4();

			push.transform =projectionView* modelMatrix;
			push.normalMatrix = obj.transform.normalMatrix();
			vkCmdPushConstants(commandBuffer,
				pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
				sizeof(SimplePushConstantData), &push);

			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}
}