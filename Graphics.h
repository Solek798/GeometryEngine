//
// Created by felix on 08.06.20.
//

#ifndef GEOMETRYENGINE_GRAPHICS_H
#define GEOMETRYENGINE_GRAPHICS_H

#include "Manageable.h"
#include "Device.h"
#include "DeviceManager.h"
#include "Pipeline.h"
#include "Command.h"

namespace geo {
    class Graphics : public Manageable {
    public:
        Graphics(VkInstance instance, VkSurfaceKHR surface);
        Graphics(VkInstance instance, VkSurfaceKHR surface, sp<DeviceManager> deviceManager);
        virtual ~Graphics() = default;

        void setup() override;
        void shutdown() override;

        void draw();

        void setDeviceManager(sp<DeviceManager> newDeviceManager);

    private:
        VkInstance instance;
        VkSurfaceKHR surface;
        VkSwapchainCreateInfoKHR swapchainCreateInfo;
        VkSwapchainKHR swapchain;
        std::vector<VkImage> images;
        std::vector<VkImageViewCreateInfo> imageViewCreateInfos;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebufferCreateInfo> framebufferCreateInfos;
        std::vector<VkFramebuffer> framebuffer;
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        VkSemaphore semaphoreImageAvailable, semaphoreRenderingDone;
        VkPresentInfoKHR presentInfo;

        sp<DeviceManager> deviceManager;
        sp<Pipeline> pipeline;
        sp<Command> command;
        std::vector<Vertex> vertecies;
    };
}


#endif //GEOMETRYENGINE_GRAPHICS_H
