#pragma once
#include "Qyhs_Device.hpp"
#include <memory>
#include <unordered_map>
namespace QYHS
{
	class QyhsDescriptorSetLayout
	{
	public:
		class Builder
		{
		public:
			Builder(QyhsDevice& qyhsDevice) :qyhsDevice(qyhsDevice) {}

			Builder& addBinding(
				uint32_t binding, VkDescriptorType descripotrType,
				VkShaderStageFlags stageFlags, uint32_t count = 1
			);
			std::unique_ptr<QyhsDescriptorSetLayout> build() const;
		private:
			QyhsDevice& qyhsDevice;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
		};

		QyhsDescriptorSetLayout(QyhsDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~QyhsDescriptorSetLayout();
		QyhsDescriptorSetLayout(const QyhsDescriptorSetLayout&) = delete;
		QyhsDescriptorSetLayout& operator=(const QyhsDescriptorSetLayout&) = delete;

		VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }


	private:
		QyhsDevice& qyhsDevice;
		VkDescriptorSetLayout descriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

		friend class QyhsDescriptorWriter;

	};

	class QyhsDescriptorPool
	{
	public:
		class Builder
		{
		public:
			Builder(QyhsDevice & device):qyhsDevice(device){}
			Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
			Builder& setMaxSets(uint32_t count);
			std::unique_ptr<QyhsDescriptorPool> build() const;

		private:
			QyhsDevice& qyhsDevice;
			std::vector<VkDescriptorPoolSize> poolSizes{};
			uint32_t maxSets = 1000;
			VkDescriptorPoolCreateFlags poolFlags = 0;
		};

		QyhsDescriptorPool(QyhsDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
			const std::vector<VkDescriptorPoolSize>& poolSizes);
		~QyhsDescriptorPool();
		QyhsDescriptorPool(const QyhsDescriptorPool&) = delete;
		QyhsDescriptorPool& operator=(const QyhsDescriptorPool&) = delete;

		bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

		void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;
		void resetPool();
		
	private:
		QyhsDevice& qyhsDevice;
		VkDescriptorPool descriptorPool;
		friend class QyhsDescriptorWriter;
	};

	class QyhsDescriptorWriter
	{
	public:
		QyhsDescriptorWriter(QyhsDescriptorSetLayout& setLayout, QyhsDescriptorPool& pool);
		QyhsDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		QyhsDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool build(VkDescriptorSet& set);
		void overwrite(VkDescriptorSet& set);

	private:
		QyhsDescriptorSetLayout& setLayout;
		QyhsDescriptorPool& pool;
		std::vector<VkWriteDescriptorSet> writes;
	};
}