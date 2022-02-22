#pragma once



#include "Qyhs_Application.hpp"
#include "Qyhs_RenderSystem.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <array>
#include <chrono>
#include <iostream>

namespace QYHS
{
	QyhsApplication::QyhsApplication()
	{
		loadGameObjects();

	}

	QyhsApplication::~QyhsApplication() {}

	void QyhsApplication::run()
	{
		QyhsCamera camera{};
		////camera.setViewDirection(glm::vec3(0.0f), glm::vec3(0.0f, 0.f, 1.7f));
		//camera.setViewTarget(glm::vec3(-1.0f, -2.f, -2.f), glm::vec3(0.0f, 0.f, 1.0f));

		auto viewerObject = QyhsGameObject::createGameObject();
		CameraMovementController cameraController{qyhsWindow};

		auto currentTime = std::chrono::high_resolution_clock::now();
		SimpleRenderSystem simpleRenderSystem{ qyhsDevice,qyhsRender.getSwapChainRenderPass() };
		while (!qyhsWindow.bShouldClose())
		{
			glfwPollEvents();

			auto newTime= std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			
			currentTime = newTime;

			cameraController.moveInPlaneXZ(qyhsWindow.getGlfwWindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);


			
			float aspect = qyhsRender.getAspectRatio();

			camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);
			if (auto commandBuffer = qyhsRender.beginFrame())
			{
				qyhsRender.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObject(commandBuffer, gameObjects,camera);
				qyhsRender.endSwapChainRenderPass(commandBuffer);
				qyhsRender.endFrame();
			}
		}

		vkDeviceWaitIdle(qyhsDevice.device());
	}

	
	void QyhsApplication::loadGameObjects()
	{

		//std::shared_ptr<QyhsModel> qyhsModel = createCubeModel(qyhsDevice, { 0.f,0.f,0.f });
		//std::string str = "models/smooth_vase.obj";
		std::string str = "models/banya.obj";
		std::shared_ptr<QyhsModel> qyhsModel = QyhsModel::createModelFromFile(qyhsDevice, str);
		auto gameobj1 = QyhsGameObject::createGameObject();
		gameobj1.model = qyhsModel;
		gameobj1.transform.translation = { -0.5f,0.9f,1.0f };
		gameobj1.transform.scale = { 0.8f,0.8f,0.8f };

		gameObjects.push_back(std::move(gameobj1));

		auto gameobj2 = QyhsGameObject::createGameObject();
		gameobj2.model = qyhsModel;
		gameobj2.transform.translation = { 0.5f,0.9f,1.0f };
		gameobj2.transform.scale = { 0.8f,0.8f,0.8f };

		gameObjects.push_back(std::move(gameobj2));
	}


}