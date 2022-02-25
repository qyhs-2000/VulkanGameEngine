#include "Qyhs_Descriptors.hpp"
#include <cassert>
#include <stdexcept>
namespace QYHS
{
	QyhsDescriptorSetLayout::Builder& QyhsDescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descripotrType, VkShaderStageFlags stageFlags, uint32_t count)
	{
		assert(bindings.count(binding) == 0 && "Binding already in use");
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descripotrType;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = stageFlags;
		bindings[binding] = layoutBinding;
		return *this;
	}
	QyhsDescriptorPool::Builder& QyhsDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
	{
		poolSizes.push_back({ descriptorType,count });
		return *this;
	}
	QyhsDescriptorPool::Builder& QyhsDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
	{
		poolFlags = flags;
		return *this;
	}
	QyhsDescriptorPool::Builder& QyhsDescriptorPool::Builder::setMaxSets(uint32_t count)
	{
		maxSets = count;
		return *this;
	}
	std::unique_ptr<QyhsDescriptorSetLayout> QyhsDescriptorSetLayout::Builder::build() const
	{
		return std::make_unique<QyhsDescriptorSetLayout>(qyhsDevice,bindings);
	}
	QyhsDescriptorSetLayout::QyhsDescriptorSetLayout(QyhsDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
		:qyhsDevice(device),bindings{bindings}
	{
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto kv : bindings)
		{
			setLayoutBindings.push_back(kv.second);
		}
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();
		if (vkCreateDescriptorSetLayout(qyhsDevice.device(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}
	QyhsDescriptorSetLayout::~QyhsDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(qyhsDevice.device(), descriptorSetLayout, nullptr);
	}
	QyhsDescriptorPool::QyhsDescriptorPool(QyhsDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes)
		:qyhsDevice(device)
	{
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.flags = poolFlags;
		if (vkCreateDescriptorPool(qyhsDevice.device(), &descriptorPoolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
	QyhsDescriptorPool::~QyhsDescriptorPool()
	{
		vkDestroyDescriptorPool(qyhsDevice.device(), descriptorPool, nullptr);
	}
	bool QyhsDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		allocInfo.descriptorSetCount = 1;
		if (vkAllocateDescriptorSets(qyhsDevice.device(), &allocInfo, &descriptor) != VK_SUCCESS)
		{
			return false;
		}
		return true;
	}
	void QyhsDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
	{
		vkFreeDescriptorSets(
			qyhsDevice.device(),
			descriptorPool,
			static_cast<uint32_t>(descriptors.size()),
			descriptors.data());
	}
	void QyhsDescriptorPool::resetPool()
	{
		vkResetDescriptorPool(qyhsDevice.device(), descriptorPool, 0);
	}
	std::unique_ptr<QyhsDescriptorPool> QyhsDescriptorPool::Builder::build() const
	{
		return std::make_unique<QyhsDescriptorPool>(qyhsDevice, maxSets, poolFlags, poolSizes);
	}

	QyhsDescriptorWriter::QyhsDescriptorWriter(QyhsDescriptorSetLayout& setLayout, QyhsDescriptorPool& pool)
		:setLayout{setLayout},pool{pool}{}
	QyhsDescriptorWriter& QyhsDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
	{
		assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");
		auto& bindingDescription = setLayout.bindings[binding];
		assert(
			bindingDescription.descriptorCount == 1 &&
			"Binding single descriptor info, but binding expects multiple");
		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;
		writes.push_back(write);
		return *this;
	}
	QyhsDescriptorWriter& QyhsDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
	{
		assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = setLayout.bindings[binding];

		assert(
			bindingDescription.descriptorCount == 1 &&
			"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}
	bool QyhsDescriptorWriter::build(VkDescriptorSet& set)
	{
		bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
		if (!success) {
			return false;
		}
		overwrite(set);
		return true;
	}
	void QyhsDescriptorWriter::overwrite(VkDescriptorSet& set)
	{
		for (auto& write : writes) {
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(pool.qyhsDevice.device(), writes.size(), writes.data(), 0, nullptr);
	}
}