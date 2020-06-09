//
// Created by felix on 18.04.20.
//

#ifndef GEOMETRYENGINE_DEVICE_H
#define GEOMETRYENGINE_DEVICE_H

#include <vector>
#include "vulkan/vulkan.h"
#include "Manageable.h"

namespace geo {
    class Device : Manageable {
    public:
        explicit Device(VkPhysicalDevice physicalDevice);
        virtual ~Device() = default;

        void setup() override;
        void shutdown() override;

        [[nodiscard]] VkPhysicalDevice getPhysicalHandle() const;
        [[nodiscard]] VkDevice getLogicalHandle() const;
        [[nodiscard]] std::vector<VkQueueFamilyProperties> getQueueFamilies() const;
        [[nodiscard]] sp<std::vector<VkQueueFamilyProperties>> getGraphicsQueueFamilies() const;

        void debugPrintDeviceProperty() const;

    private:
        VkDevice logicalHandle;
        VkPhysicalDevice physicalHandle;

        std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        std::vector<std::vector<VkQueue>> queues;
    };
}


#endif //GEOMETRYENGINE_DEVICE_H
