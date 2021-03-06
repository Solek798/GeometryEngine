//
// Created by felix on 12.04.20.
//

#include <vulkan/vulkan.h>
#include <iostream>
#include "DeviceManager.h"

geo::DeviceManager::DeviceManager(VkInstance& instance)
    : instance(instance)
    , currentDeviceIndex(0) { }

void geo::DeviceManager::setup() {
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

#ifdef GEO_STATUS_NOTIFICATIONS
    std::cout << "#> DeviceManager ready!" << std::endl;
#endif

    for (int i=0 ; i<deviceCount ; i++) {
        devices.emplace_back(std::make_shared<Device>(physicalDevices[0]));
        devices.back()->setup();
    }
}

void geo::DeviceManager::shutdown() {
    for (const auto& device : devices) {
        device->shutdown();
    }
}

int geo::DeviceManager::getDeviceCount() const {
    return devices.size();
}

sp<geo::Device> geo::DeviceManager::getDevice(int index) const {
    return devices[index];
}

sp<geo::Device> geo::DeviceManager::getCurrentDevice() const {
    return devices[currentDeviceIndex];
}

void geo::DeviceManager::setCurrentDevice(int index) {
    currentDeviceIndex = index;
}

void geo::DeviceManager::waitForAllDevices() const {
    for (const auto& device : devices) {
        vkDeviceWaitIdle(device->getLogicalHandle());
    }
}




