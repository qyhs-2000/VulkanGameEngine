#pragma once
#include "Qyhs_Model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <unordered_map>
namespace QYHS
{

	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f,1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};
	class QyhsGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, QyhsGameObject>;

		static QyhsGameObject createGameObject() {
			static id_t currentId = 0;
			return QyhsGameObject{ currentId++ };
		}

		QyhsGameObject(const QyhsGameObject&) = delete;
		QyhsGameObject& operator=(const QyhsGameObject&) = delete;
		QyhsGameObject(QyhsGameObject&&) = default;
		QyhsGameObject& operator=(QyhsGameObject&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<QyhsModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		QyhsGameObject(id_t objId) : id{ objId } {}

		id_t id;
	};
}