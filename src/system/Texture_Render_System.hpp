#pragma once


#include "Qyhs_Window.hpp"
#include "Qyhs_pipeline.hpp"
#include "Qyhs_Renderer.hpp"

//#include "Qyhs_Application.hpp"
#include <glm/gtc/constants.hpp>
#include <memory>
#include <array>
#include <iostream>
#include <memory>
namespace QYHS
{
	struct TexturePushConstantData
	{
		glm::mat4 modelMatrix{ 1.f };
	};
	
	class TextureRenderSystem
	{
	public:
		TextureRenderSystem(QyhsDevice &device,VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout, VkCommandBuffer  commandBuffer);
		~TextureRenderSystem();

		VkDescriptorImageInfo getDescriptorImageInfo();

		TextureRenderSystem(const TextureRenderSystem &) = delete;
		TextureRenderSystem & operator=(const TextureRenderSystem&) = delete;

		void render(FrameInfo & frameInfo);

		std::vector<VkVertexInputAttributeDescription> TexgetAttributeDescriptions();

	private:

		void createPipeline(VkRenderPass renderPass);
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createTextureImageView();
		void createTextureImage(VkCommandBuffer& commandBuffer);
		void createTextureSampler();
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		VkImageView createImageView(VkImage image, VkFormat format);
		QyhsDevice &qyhsDevice;
		

		VkImage textureImage;
		VkImageView textureImageView;
		VkDeviceMemory textureImageMemory;
		VkSampler textureSampler;
		
		std::unique_ptr<QyhsPipeline> qyhsPipeline;
		VkPipelineLayout pipelineLayout;
	};
}