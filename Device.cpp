//
// Created by felix on 18.04.20.
//

#include "Device.h"
#include "GlobaleScope.h"
#include <iostream>

geo::Device::Device(VkPhysicalDevice physicalDevice) : physicalHandle(physicalDevice) { }

void geo::Device::setup() {
    VkResult result;
    uint32_t amountQueueFamilies = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalHandle, &amountQueueFamilies, nullptr);
    queueFamilyProperties.resize(amountQueueFamilies);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalHandle, &amountQueueFamilies, queueFamilyProperties.data());

    //TODO: dynamic Priorities
    const std::vector<float> priorities(100, 1.0f);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(amountQueueFamilies);
    for (int i=0 ; i<amountQueueFamilies ; i++) {
        queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[i].queueFamilyIndex = i;
        queueCreateInfos[i].pNext = nullptr;
        queueCreateInfos[i].flags = 0;
        queueCreateInfos[i].queueCount = queueFamilyProperties[i].queueCount;
        queueCreateInfos[i].pQueuePriorities = priorities.data();
    }

    VkPhysicalDeviceFeatures usedFeatures{};

    // TODO: BAD INIT
    const std::vector<const char*> deviceExtentions {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = amountQueueFamilies;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.enabledExtensionCount = deviceExtentions.size();
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtentions.data();
    deviceCreateInfo.pEnabledFeatures = &usedFeatures;

    result = vkCreateDevice(physicalHandle, &deviceCreateInfo, nullptr, &logicalHandle);
    VK_ASSERT(result);

    for (int i=0 ; i<queueFamilyProperties.size() ; i++) {
        queues.emplace_back();
        for (int j=0 ; j<queueFamilyProperties[i].queueCount ; j++) {
            queues.back().emplace_back();
            vkGetDeviceQueue(logicalHandle, i, j, &(queues.back().back()));
        }
    }

#ifdef GEO_STATUS_NOTIFICATIONS
    std::cout << "#> Device ready!" << std::endl;
#endif
}

void geo::Device::shutdown() {
    vkDeviceWaitIdle(logicalHandle);

    vkDestroyDevice(logicalHandle, nullptr);
}

void geo::Device::debugPrintDeviceProperty() const {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalHandle, &properties);

    uint32_t apiVersion = properties.apiVersion;

    std::cout << "##############################################################################" << std::endl;
    std::cout << "Name: " << properties.deviceName << std::endl;
    switch (properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER: std::cout << "Device Type: " << "VK_PHYSICAL_DEVICE_TYPE_OTHER" << std::endl; break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: std::cout << "Device Type: " << "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU" << std::endl; break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: std::cout << "Device Type: " << "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU" << std::endl; break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: std::cout << "Device Type: " << "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU" << std::endl; break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU: std::cout << "Device Type: " << "VK_PHYSICAL_DEVICE_TYPE_CPU" << std::endl; break;
        case VK_PHYSICAL_DEVICE_TYPE_RANGE_SIZE: std::cout << "Device Type: " << "VK_PHYSICAL_DEVICE_TYPE_RANGE_SIZE" << std::endl; break;
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM: std::cout << "Device Type: " << "VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM" << std::endl; break;
    }
    std::cout << "Vendor ID: " << properties.vendorID << std::endl;
    std::cout << "Device ID: " << properties.deviceID << std::endl;
    std::cout << "API Version: " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) << "." << VK_VERSION_PATCH(apiVersion) << std::endl;
    std::cout << "Pipeline Cache UUID: " << properties.pipelineCacheUUID << std::endl;
    std::cout << "DiscreteQueuePriorities: " << properties.limits.discreteQueuePriorities << std::endl;
    std::cout << std::endl;
    std::cout << "Amount Queue Families: " << queueFamilyProperties.size() << std::endl;
    for (const auto& property : queueFamilyProperties) {
        std::cout << "Type: " << std::endl;
        std::cout << "\tVK_QUEUE_GRAPHICS_BIT:\t\t\t" << ((property.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << std::endl;
        std::cout << "\tVK_QUEUE_COMPUTE_BIT:\t\t\t" << ((property.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << std::endl;
        std::cout << "\tVK_QUEUE_TRANSFER_BIT:\t\t\t" << ((property.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << std::endl;
        std::cout << "\tVK_QUEUE_SPARSE_BINDING_BIT:\t" << ((property.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << std::endl;
        std::cout << "\tVK_QUEUE_PROTECTED_BIT:\t\t\t" << ((property.queueFlags & VK_QUEUE_PROTECTED_BIT) != 0) << std::endl;

        std::cout << "Amount of Queues: " << property.queueCount << std::endl;
        std::cout << "Granularity: "
            << property.minImageTransferGranularity.width << ", "
            << property.minImageTransferGranularity.height << ", "
            << property.minImageTransferGranularity.depth << ", " << std::endl;
    }

    std::cout << std::endl;

    std::cout << std::endl;
}

VkPhysicalDevice geo::Device::getPhysicalHandle() const {
    return physicalHandle;
}

VkDevice geo::Device::getLogicalHandle() const {
    return logicalHandle;
}

std::vector<VkQueueFamilyProperties> geo::Device::getQueueFamilies() const {
    return queueFamilyProperties;
}

sp<std::vector<VkQueueFamilyProperties>> geo::Device::getGraphicsQueueFamilies() const {
    auto result = std::make_shared<std::vector<VkQueueFamilyProperties>>();

    for (const auto& property : queueFamilyProperties) {
        if ((property.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
            result->emplace_back(property);
        }
    }

    return result;
}



