//
// Created by felix on 08.06.20.
//

#ifndef GEOMETRYENGINE_GRAPHICS_H
#define GEOMETRYENGINE_GRAPHICS_H

#include "Manageable.h"
#include "Device.h"

namespace geo {
    class Graphics : public Manageable {
    public:
        Graphics(VkInstance instance, VkSurfaceKHR surface);
        Graphics(VkInstance instance, VkSurfaceKHR surface, sp<Device> currentDevice);
        virtual ~Graphics() = default;

        void setup() override;
        void shutdown() override;

        void setCurrentDevice(sp<Device> newDevice);
        [[nodiscard]] Device& getCurrentDevice() const;

    private:
        VkInstance instance;
        VkSurfaceKHR surface;
        VkSwapchainCreateInfoKHR swapchainCreateInfo;
        VkSwapchainKHR swapchain;
        std::vector<VkImageView> imageViews;

        sp<Device> currentDevice;
    };
}


#endif //GEOMETRYENGINE_GRAPHICS_H
