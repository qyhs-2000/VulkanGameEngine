#pragma once
#include "Qyhs_Window.hpp"
#include "Qyhs_SwapChain.hpp"
#include <glm/glm.hpp>
#include <cassert>
#include <memory>
namespace QYHS
{
	


	class QyhsRenderer
	{
	public:
		QyhsRenderer(QyhsWindow &window,QyhsDevice &device);
		~QyhsRenderer();


		

		QyhsRenderer(const QyhsRenderer &) = delete;
		QyhsRenderer & operator=(const QyhsRenderer&) = delete;

		

		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(isFrameStarted&&"Cannot get command buffer when the frame is not in frame!");
			return commandBuffers[currentImageIndex];
		}

		VkRenderPass getSwapChainRenderPass() const
		{
			return qyhsSwapChain->getRenderPass();
		}

		VkCommandBuffer beginFrame();
		float getAspectRatio() { return qyhsSwapChain->extentAspectRatio(); }

		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffrrs();
		void freeCommandBuffer();
		void reCreateSwapChain();

		QyhsWindow &qyhsWindow;
		QyhsDevice &qyhsDevice;
		std::unique_ptr<QyhsSwapChain> qyhsSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;
		uint32_t currentImageIndex;
		bool isFrameStarted = false;

	};
}