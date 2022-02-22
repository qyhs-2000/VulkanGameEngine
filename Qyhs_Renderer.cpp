#include "Qyhs_Renderer.hpp"

#pragma once



#include "Qyhs_Application.hpp"
#include <glm/gtc/constants.hpp>
#include <memory>
#include <array>
#include <iostream>

namespace QYHS
{
	QyhsRenderer::QyhsRenderer(QyhsWindow &window, QyhsDevice &device)
		:qyhsWindow(window),qyhsDevice(device)
	{
		
		
		reCreateSwapChain();
		createCommandBuffrrs();
	}

	QyhsRenderer::~QyhsRenderer()
	{
		freeCommandBuffer();
	}

	
	VkCommandBuffer QyhsRenderer::beginFrame()
	{
		assert(!isFrameStarted&&"Cannot call the beginFrame when the frame is in progress!");

		auto result = qyhsSwapChain->acquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			reCreateSwapChain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}
		isFrameStarted = true;
		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin command buffer!");
		}
		return commandBuffer;

	}

	void QyhsRenderer::endFrame()
	{
		assert(isFrameStarted&&"Cannot call the endFrame when the frame is not in progress!");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}

		auto result = qyhsSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || qyhsWindow.wasWindowResizedFlag())
		{
			qyhsWindow.resetWindowResizedFlag();
			reCreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}
		
		isFrameStarted = false;
		
	}

	void QyhsRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted&&"Cannot call the beginFrame when the frame is not in progress!");
		assert(commandBuffer==getCurrentCommandBuffer()&&"Cannot  begin Render Pass from a different frame!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = qyhsSwapChain->getRenderPass();
		renderPassInfo.framebuffer = qyhsSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = qyhsSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f };
		clearValues[1].depthStencil = { 1.f,0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(qyhsSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(qyhsSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor = { {0,0},qyhsSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void QyhsRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted&&"Cannot call the beginFrame when the frame is in progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Cannot  end Render Pass from a different frame!");

		vkCmdEndRenderPass(commandBuffer);
	}

	void QyhsRenderer::createCommandBuffrrs()
	{

		commandBuffers.resize(qyhsSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = qyhsDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(qyhsDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffer!");
		}


	}
	/*创建pipeline*/
	
	void QyhsRenderer::freeCommandBuffer()
	{
		vkFreeCommandBuffers(qyhsDevice.device(), qyhsDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	


	/*创建PipelineLayout*/
	
	
	

	void QyhsRenderer::reCreateSwapChain()
	{
		auto extent = qyhsWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = qyhsWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(qyhsDevice.device());

		if (qyhsSwapChain == nullptr)
		{
			qyhsSwapChain = std::make_unique<QyhsSwapChain>(qyhsDevice, extent);
		}
		else
		{
			qyhsSwapChain = std::make_unique<QyhsSwapChain>(qyhsDevice, extent, std::move(qyhsSwapChain));
			if (qyhsSwapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffer();
				createCommandBuffrrs();
			}
		}

		//createPipeline();
	}

}