#pragma once

//lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <memory>
#include "glm/glm.hpp"

#include "Qyhs_Device.hpp"

namespace QYHS
{
	class QyhsModel
	{
	public:
		struct Vertex
		{
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator ==(const Vertex& other) const 
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}

		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel( std::string& filePath);
		};
		QyhsModel(QyhsDevice & device, const QyhsModel::Builder & builder);
		~QyhsModel();

		QyhsModel(const QyhsModel &) = delete;
		QyhsModel & operator=(const QyhsModel&) = delete;

		
		
		static std::unique_ptr<QyhsModel> createModelFromFile(QyhsDevice & device,  std::string & filepath);


		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		QyhsDevice & qyhsDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;

		bool hasIndexBuffer = false;

		void createVertexBuffers(const std::vector<Vertex> & vertics);
		void createIndexBuffers(const std::vector<uint32_t> & indices);
	};
}