//
// Created by felix on 08.06.20.
//

#include "Graphics.h"
#include "vulkan/vulkan.h"
#include "IO/FileInput.h"
#include <utility>
#include <iostream>

geo::Graphics::Graphics(VkInstance instance, VkSurfaceKHR surface)
    : instance(instance)
    , surface(surface)
    , deviceManager(nullptr)
    , pipeline(nullptr) { }

geo::Graphics::Graphics(VkInstance instance, VkSurfaceKHR surface, sp<DeviceManager> deviceManager)
    : instance(instance)
    , surface(surface)
    , deviceManager(std::move(deviceManager))
    , pipeline(nullptr) { }

void geo::Graphics::setup() {
    auto physicalHandle = deviceManager->getCurrentDevice()->getPhysicalHandle();
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalHandle, surface, &surfaceCapabilities);

#ifdef GEO_DEBUG
    std::cout << surfaceCapabilities.minImageCount << std::endl;
    std::cout << surfaceCapabilities.maxImageCount << std::endl;
    std::cout << surfaceCapabilities.currentExtent.width << ", " << surfaceCapabilities.currentExtent.height << std::endl;
    std::cout << surfaceCapabilities.minImageExtent.width << ", " << surfaceCapabilities.minImageExtent.height << std::endl;
    std::cout << surfaceCapabilities.maxImageExtent.width << ", " << surfaceCapabilities.maxImageExtent.height << std::endl;
    std::cout << surfaceCapabilities.maxImageArrayLayers << std::endl;
    std::cout << surfaceCapabilities.supportedTransforms << std::endl;
    std::cout << surfaceCapabilities.currentTransform << std::endl;
    std::cout << surfaceCapabilities.supportedCompositeAlpha << std::endl;
    std::cout << surfaceCapabilities.supportedUsageFlags << std::endl;
#endif

    uint32_t amountOfFormats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalHandle, surface, &amountOfFormats, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats{ amountOfFormats };
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalHandle, surface, &amountOfFormats, surfaceFormats.data());

#ifdef GEO_DEBUG
    std::cout << "Formats: " << std::endl;
    for (const auto& format : surfaceFormats) {
        std::cout << format.format << std::endl;
    }
#endif

    uint32_t amountOfPresentModes = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalHandle, surface, &amountOfPresentModes, nullptr);
    std::vector<VkPresentModeKHR> presentModes{ amountOfPresentModes };
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalHandle, surface, &amountOfPresentModes, presentModes.data());

#ifdef GEO_DEBUG
    std::cout << "Presentation Modes: " << std::endl;
    for (const auto& modes : presentModes) {
        std::cout << modes << std::endl;
    }
#endif

    // the validation layer wants this to get validated. TODO: replace with proper solution
    for (int i=0 ; i<deviceManager->getCurrentDevice()->getQueueFamilies().size() ; i++) {
        VkBool32 valid;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalHandle, i, surface, &valid);

        //if (!valid) throw 1; // TODO: insert Exception
    }


    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = surfaceCapabilities.maxImageCount >= 3 ? 3 : surfaceCapabilities.minImageCount;
    swapchainCreateInfo.imageFormat = surfaceFormats[0].format; // TODO: BAD INIT
    swapchainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace; // TODO: BAD INIT
    swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 0;
    swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    vkCreateSwapchainKHR(logicalHandle, &swapchainCreateInfo, nullptr, &swapchain);

    uint32_t amountOfImages = 0;
    vkGetSwapchainImagesKHR(logicalHandle, swapchain, &amountOfImages, nullptr);
    std::vector<VkImage> images{amountOfImages};
    vkGetSwapchainImagesKHR(logicalHandle, swapchain, &amountOfImages, images.data());

    VkImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = VK_NULL_HANDLE;
    imageViewCreateInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = swapchainCreateInfo.imageFormat;
    imageViewCreateInfo.components = VkComponentMapping{VK_COMPONENT_SWIZZLE_IDENTITY,
                                                        VK_COMPONENT_SWIZZLE_IDENTITY,
                                                        VK_COMPONENT_SWIZZLE_IDENTITY,
                                                        VK_COMPONENT_SWIZZLE_IDENTITY};
    imageViewCreateInfo.subresourceRange = VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT,
                                                                   0,
                                                                   1,
                                                                   0,
                                                                   1};

    imageViews.resize(images.size());

    for (int i=0 ; i<images.size() ; i++) {
        imageViewCreateInfo.image = images[i];
        vkCreateImageView(logicalHandle, &imageViewCreateInfo, nullptr, &imageViews[i]);
    }

    pipeline = std::make_shared<Pipeline>(deviceManager);
    pipeline->setup();
}

void geo::Graphics::shutdown() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    pipeline->shutdown();

    for (const auto& imageView : imageViews) {
        vkDestroyImageView(logicalHandle, imageView , nullptr);
    }
    vkDestroySwapchainKHR(logicalHandle, swapchain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

void geo::Graphics::setDeviceManager(sp<geo::DeviceManager> newDeviceManager) {
    deviceManager = std::move(newDeviceManager);
}

