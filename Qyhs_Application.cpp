

#include "Qyhs_Application.hpp"

#include "Camera_movement_controller.hpp"
#include "Qyhs_buffer.hpp"
#include "Qyhs_camera.hpp"
#include "Qyhs_RenderSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace QYHS {

    struct GlobalUbo {
        glm::mat4 projectionView{ 1.f };
        glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
        glm::vec3 lightPosition{ 1.f,-0.5f,-0.5f };
        alignas(16) glm::vec4 lightColor{ 1.f };  // w is light intensity
        //glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.f, -3.f, -1.f });
    };

    QyhsApplication::QyhsApplication() {
        globalPool =
            QyhsDescriptorPool::Builder(qyhsDevice)
            .setMaxSets(QyhsSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, QyhsSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        loadGameObjects();
    }

    QyhsApplication::~QyhsApplication() {}

    void QyhsApplication::run() {
        std::vector<std::unique_ptr<QyhsBuffer>> uboBuffers(QyhsSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<QyhsBuffer>(
                qyhsDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout =
            QyhsDescriptorSetLayout::Builder(qyhsDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(QyhsSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            QyhsDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{
            qyhsDevice,
            qyhsRender.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout() };
        QyhsCamera camera{};

        auto viewerObject = QyhsGameObject::createGameObject();
        CameraMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!qyhsWindow.bShouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(qyhsWindow.getGlfwWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = qyhsRender.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if (auto commandBuffer = qyhsRender.beginFrame()) {
                int frameIndex = qyhsRender.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex] };

                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                qyhsRender.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObject(frameInfo, gameObjects);
                qyhsRender.endSwapChainRenderPass(commandBuffer);
                qyhsRender.endFrame();
            }
        }

        vkDeviceWaitIdle(qyhsDevice.device());
    }

    void QyhsApplication::loadGameObjects() {
        std::shared_ptr<QyhsModel> QyhsModel =
            QyhsModel::createModelFromFile(qyhsDevice, "models/banya.obj");
        auto flatVase = QyhsGameObject::createGameObject();
        flatVase.model = QyhsModel;
        flatVase.transform.translation = { -.5f, .5f, .5f };
        flatVase.transform.scale = { .6f, .6f, .6f };
        gameObjects.push_back(std::move(flatVase));

        QyhsModel = QyhsModel::createModelFromFile(qyhsDevice, "models/leidian.obj");
        auto smoothVase = QyhsGameObject::createGameObject();
        smoothVase.model = QyhsModel;
        smoothVase.transform.translation = { .5f, .5f, .5f };
        smoothVase.transform.scale = { .5f, .5f, .5f };
        gameObjects.push_back(std::move(smoothVase));

        QyhsModel = QyhsModel::createModelFromFile(qyhsDevice, "models/Quad.obj");
        auto floor = QyhsGameObject::createGameObject();
        floor.model = QyhsModel;
        floor.transform.translation = { .0f, .5f, .5f };
        floor.transform.scale = { 3.5f, .5f, 3.5f };
        gameObjects.push_back(std::move(floor));
    }

}  // namespace Qyhs