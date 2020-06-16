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
    usedFeatures.samplerAnisotropy = VK_TRUE;

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

const std::vector<std::vector<VkQueue>> &geo::Device::getQueues() const {
    return queues;
}

uint32_t geo::Device::findMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalHandle, &physicalDeviceMemoryProperties);

    for (uint32_t i = 0 ; i < physicalDeviceMemoryProperties.memoryTypeCount ; i++) {
        if (typeFilter & (1 << i) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    // TODO: insert valid Exception
    return -1;
}

void geo::Device::createBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags bufferUsageFlags, VkBuffer &buffer,
                               VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceMemory &deviceMemory) {
    // Start Create Buffer
    VkBufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = deviceSize;
    bufferCreateInfo.usage = bufferUsageFlags;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;
    vkCreateBuffer(logicalHandle, &bufferCreateInfo, nullptr, &buffer);

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(logicalHandle, buffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findMemoryTypeIndex(
            memoryRequirements.memoryTypeBits,
            memoryPropertyFlags);

    //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    vkAllocateMemory(logicalHandle, &memoryAllocateInfo, nullptr, &deviceMemory);

    vkBindBufferMemory(logicalHandle, buffer, deviceMemory, 0);

}



