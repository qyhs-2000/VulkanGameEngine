#pragma once

#include "Qyhs_Device.hpp"
#include "Qyhs_Model.hpp"
#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
namespace QYHS
{
	
	struct PipelineConfigInfo
	{
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo() = default;

		
		std::vector<VkVertexInputBindingDescription> bindingDescriptors{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptors{};


		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class QyhsPipeline
	{
	public:
		QyhsPipeline( QyhsDevice & device,
			const std::string & vertFilePath, 
			const std::string & fragFilePath,
			 const PipelineConfigInfo& configInfo);


		~QyhsPipeline();

		QyhsPipeline(const QyhsPipeline &) = delete;
		QyhsPipeline & operator =(const QyhsPipeline &) = delete;

		void bind(VkCommandBuffer commandBuffer);

		static void  defaultPipelineConfigInfo(PipelineConfigInfo & config);
	private:

		void createShaderModule(std::vector<char> & code, VkShaderModule * shaderModule);
		static std::vector<char> readFile(const std::string & fileName);
		void createGraphicsPipeline(const std::string& vertFilePath,
			const std::string& fragFilePath,
			const  PipelineConfigInfo & configInfo);

		
		QyhsDevice &qyhsDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}