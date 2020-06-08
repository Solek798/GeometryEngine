//
// Created by felix on 08.06.20.
//

#ifndef GEOMETRYENGINE_GRAPHICS_H
#define GEOMETRYENGINE_GRAPHICS_H

#include "Manageable.h"
#include "Device.h"
#include "DeviceManager.h"
#include "Pipeline.h"

namespace geo {
    class Graphics : public Manageable {
    public:
        Graphics(VkInstance instance, VkSurfaceKHR surface);
        Graphics(VkInstance instance, VkSurfaceKHR surface, sp<DeviceManager> deviceManager);
        virtual ~Graphics() = default;

        void setup() override;
        void shutdown() override;

        void setDeviceManager(sp<DeviceManager> newDeviceManager);

    private:
        VkInstance instance;
        VkSurfaceKHR surface;
        VkSwapchainCreateInfoKHR swapchainCreateInfo;
        VkSwapchainKHR swapchain;
        std::vector<VkImageView> imageViews;

        sp<DeviceManager> deviceManager;
        sp<Pipeline> pipeline;
    };
}


#endif //GEOMETRYENGINE_GRAPHICS_H
