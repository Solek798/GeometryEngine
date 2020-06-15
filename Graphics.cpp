//
// Created by felix on 08.06.20.
//

#include "Graphics.h"
#include "vulkan/vulkan.h"
#include "IO/FileInput.h"
#include <utility>
#include <iostream>
#include <memory>

geo::Graphics::Graphics(VkInstance instance, VkSurfaceKHR surface)
    : instance(instance)
    , surface(surface)
    , deviceManager(nullptr)
    , pipeline(nullptr) {

    vertecies.emplace_back(-0.5f, 0.5f, 0.0f, 1.0f);
    vertecies.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f);
    vertecies.emplace_back(0.5f, 0.5f, 1.0f, 1.0f);
    vertecies.emplace_back(0.5f, -0.5f, 1.0f, 0.0f);
}

geo::Graphics::Graphics(VkInstance instance, VkSurfaceKHR surface, sp<DeviceManager> deviceManager)
    : instance(instance)
    , surface(surface)
    , deviceManager(std::move(deviceManager))
    , pipeline(nullptr) {

    vertecies.emplace_back(-0.5f, 0.5f, 0.0f, 1.0f);
    vertecies.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f);
    vertecies.emplace_back(0.5f, 0.5f, 1.0f, 1.0f);
    vertecies.emplace_back(0.5f, -0.5f, 1.0f, 0.0f);
}

void geo::Graphics::setup() {
    auto physicalHandle = deviceManager->getCurrentDevice()->getPhysicalHandle();
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalHandle, surface, &surfaceCapabilities);

#ifdef GEO_DEBUG_STATS
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

#ifdef GEO_DEBUG_STATS
    std::cout << "Formats: " << std::endl;
    for (const auto& format : surfaceFormats) {
        std::cout << format.format << std::endl;
    }
#endif

    uint32_t amountOfPresentModes = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalHandle, surface, &amountOfPresentModes, nullptr);
    std::vector<VkPresentModeKHR> presentModes{ amountOfPresentModes };
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalHandle, surface, &amountOfPresentModes, presentModes.data());

#ifdef GEO_DEBUG_STATS
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
    images.resize(amountOfImages);
    vkGetSwapchainImagesKHR(logicalHandle, swapchain, &amountOfImages, images.data());


    imageViews.resize(images.size());
    imageViewCreateInfos.resize(images.size());

    for (int i=0 ; i<images.size() ; i++) {
        imageViewCreateInfos[i].sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfos[i].pNext = nullptr;
        imageViewCreateInfos[i].flags = 0;
        imageViewCreateInfos[i].image = VK_NULL_HANDLE;
        imageViewCreateInfos[i].viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfos[i].format = swapchainCreateInfo.imageFormat;
        imageViewCreateInfos[i].image = images[i];
        imageViewCreateInfos[i].components = VkComponentMapping{VK_COMPONENT_SWIZZLE_IDENTITY,
                                                               VK_COMPONENT_SWIZZLE_IDENTITY,
                                                               VK_COMPONENT_SWIZZLE_IDENTITY,
                                                               VK_COMPONENT_SWIZZLE_IDENTITY};
        imageViewCreateInfos[i].subresourceRange = VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT,
                                                                       0,
                                                                       1,
                                                                       0,
                                                                       1};

        vkCreateImageView(logicalHandle, &(imageViewCreateInfos[i]), nullptr, &imageViews[i]);
    }

#ifdef GEO_STATUS_NOTIFICATIONS
    std::cout << "#> Graphics ready!" << std::endl;
#endif

    descriptorSetLayoutBinding.binding = 0;
    descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    descriptorSetLayoutBinding.descriptorCount = 1;
    descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = nullptr;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;

    vkCreateDescriptorSetLayout(logicalHandle, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout);



    descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSize.descriptorCount = 1;

    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.flags = 0;
    descriptorPoolCreateInfo.maxSets = 1;
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

    vkCreateDescriptorPool(logicalHandle, &descriptorPoolCreateInfo, nullptr, &descriptorPool);

    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pNext = nullptr;
    descriptorSetAllocateInfo.descriptorPool = descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = 1;
    descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

    vkAllocateDescriptorSets(logicalHandle, &descriptorSetAllocateInfo, &descriptorSet);



    pipeline = std::make_shared<Pipeline>(deviceManager, descriptorSetLayout);
    pipeline->setup();

    framebufferCreateInfos.resize(imageViews.size());
    framebuffer.resize(framebufferCreateInfos.size());
    for (int i=0 ; i<framebufferCreateInfos.size() ; i++) {
        framebufferCreateInfos[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfos[i].pNext = nullptr;
        framebufferCreateInfos[i].flags = 0;
        framebufferCreateInfos[i].renderPass = pipeline->getRenderPass();
        framebufferCreateInfos[i].attachmentCount = 1;
        framebufferCreateInfos[i].pAttachments = &(imageViews[i]);
        framebufferCreateInfos[i].width = WINDOW_WIDTH;
        framebufferCreateInfos[i].height = WINDOW_HEIGHT;
        framebufferCreateInfos[i].layers = 1;

        vkCreateFramebuffer(logicalHandle, &(framebufferCreateInfos[i]), nullptr, &(framebuffer[i]));
    }

    command = std::make_shared<Command>(deviceManager, pipeline, framebuffer);
    command->setup();
    command->record();
    command->mapMemory(vertecies);

    testImage = std::make_shared<Image>("../resources/rocket.png");
    testImage->upload(deviceManager->getCurrentDevice(), command);

    descriptorImageInfo.sampler = testImage->getSampler();
    descriptorImageInfo.imageView = testImage->getImageView();
    descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = descriptorSet;
    writeDescriptorSet.dstBinding = 0;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSet.pImageInfo = &descriptorImageInfo;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(logicalHandle, 1, &writeDescriptorSet, 0, nullptr);

    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    vkCreateSemaphore(logicalHandle, &semaphoreCreateInfo, nullptr, &semaphoreImageAvailable);
    vkCreateSemaphore(logicalHandle, &semaphoreCreateInfo, nullptr, &semaphoreRenderingDone);
}

void geo::Graphics::shutdown() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    vkDestroySemaphore(logicalHandle, semaphoreImageAvailable, nullptr);
    vkDestroySemaphore(logicalHandle, semaphoreRenderingDone, nullptr);

    command->shutdown();

    for (const auto& buffer : framebuffer) {
        vkDestroyFramebuffer(logicalHandle, buffer, nullptr);
    }

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

void geo::Graphics::draw() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();
    uint32_t imageIndex = 0;

    vkAcquireNextImageKHR(logicalHandle, swapchain, std::numeric_limits<uint64_t>::max(), semaphoreImageAvailable, VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &semaphoreImageAvailable;
    std::vector<VkPipelineStageFlags> flags {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = flags.data();
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(command->getCommandBuffers()[imageIndex]);
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &semaphoreRenderingDone;

    vkQueueSubmit(deviceManager->getCurrentDevice()->getQueues()[0][0], 1, &submitInfo, VK_NULL_HANDLE);

    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &semaphoreRenderingDone;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;


    vkQueuePresentKHR(deviceManager->getCurrentDevice()->getQueues()[0][0], &presentInfo);
}


