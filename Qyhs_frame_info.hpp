#pragma once
#include "Qyhs_Camera.hpp"
#include <vulkan/vulkan.h>
namespace QYHS
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		QyhsCamera& camera;
		VkDescriptorSet globalDescriptorSet;
	};
}