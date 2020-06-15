//
// Created by felix on 13.06.20.
//

#ifndef GEOMETRYENGINE_IMAGE_H
#define GEOMETRYENGINE_IMAGE_H

#include "stb/stb_image.h"

#include <vulkan/vulkan.h>
#include <cstring>
#include "GlobaleScope.h"
#include "Device.h"
#include "Command.h"

namespace geo {
    class Image {
    public:
        explicit Image(const char* path){
            uploaded = false;
            width = -1;
            height = -1;
            channels = -1;
            pixelData = nullptr;

            pixelData = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

            if (pixelData == nullptr) {
                //insert propper exception
                throw 1;
            }
        };

        virtual ~Image(){
            if (pixelData != nullptr) {
                stbi_image_free(pixelData);
            }

            if (uploaded) {
                auto logicalHandle = this->device->getLogicalHandle();

                vkDestroySampler(logicalHandle, sampler, nullptr);
                vkDestroyImageView(logicalHandle, vkImageView, nullptr);
                vkDestroyImage(logicalHandle, vkImage, nullptr);
                vkFreeMemory(logicalHandle, memory, nullptr);

                uploaded = false;
            }
        };

        [[nodiscard]] int getWidth() const{
            return width;
        };
        [[nodiscard]] int getHeight() const{
            return height;
        };
        [[nodiscard]] int getChannels() const{
            return STBI_rgb_alpha;
        };
        [[nodiscard]] stbi_uc* getData() const{
            return pixelData;
        };
        [[nodiscard]] int getSize() const{
            if (pixelData == nullptr) {
                return -1;
            }

            return width * height * getChannels();
        };
        VkSampler getSampler() {
            return sampler;
        }
        VkImageView getImageView() {
            return vkImageView;
        }

        void upload(sp<Device> device, sp<Command> command) {
            this->device = std::move(device);
            auto logicalHandle = this->device->getLogicalHandle();

            VkDeviceSize size = getSize();

            // Start Create Buffer
            VkBufferCreateInfo bufferCreateInfo;
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.pNext = nullptr;
            bufferCreateInfo.flags = 0;
            bufferCreateInfo.size = getSize();
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
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
            memoryAllocateInfo.memoryTypeIndex = this->device->findMemoryTypeIndex(
                    memoryRequirements.memoryTypeBits,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            vkAllocateMemory(logicalHandle, &memoryAllocateInfo, nullptr, &memory);

            vkBindBufferMemory(logicalHandle, buffer, memory, 0);

            void* data = nullptr;
            vkMapMemory(logicalHandle, memory, 0, bufferCreateInfo.size, 0, &data);

            memcpy(data, (void*)getData(), bufferCreateInfo.size);

            vkUnmapMemory(logicalHandle, memory);
            // End Create Buffer

            VkImageCreateInfo imageCreateInfo;
            imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageCreateInfo.pNext = nullptr;
            imageCreateInfo.flags = 0;
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM; // Format from STB
            imageCreateInfo.extent = {static_cast<uint32_t>(getWidth()), static_cast<uint32_t>(getHeight()), 1};
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreateInfo.queueFamilyIndexCount = 0;
            imageCreateInfo.pQueueFamilyIndices = nullptr;
            imageCreateInfo.initialLayout = vkImageLayout;

            vkCreateImage(logicalHandle, &imageCreateInfo, nullptr, &vkImage);

            vkBindImageMemory(logicalHandle, vkImage, memory, 0);

            changeLayout(command, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            writeBufferToImage(command);
            changeLayout(command, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            vkDestroyBuffer(logicalHandle, buffer, nullptr);
            vkFreeMemory(logicalHandle, memory, nullptr);

            VkImageViewCreateInfo imageViewCreateInfo;
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.pNext = nullptr;
            imageViewCreateInfo.flags = 0;
            imageViewCreateInfo.image = vkImage;
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM; // Format from STB
            imageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY,
                                              VK_COMPONENT_SWIZZLE_IDENTITY,
                                              VK_COMPONENT_SWIZZLE_IDENTITY,
                                              VK_COMPONENT_SWIZZLE_IDENTITY};
            imageViewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT,
                                                    0,
                                                    1,
                                                    0,
                                                    1};

            vkCreateImageView(logicalHandle, &imageViewCreateInfo, nullptr, &vkImageView);

            VkSamplerCreateInfo samplerCreateInfo;
            samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerCreateInfo.pNext = nullptr;
            samplerCreateInfo.flags = 0;
            samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
            samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
            samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.mipLodBias = 0;
            samplerCreateInfo.anisotropyEnable = VK_TRUE;
            samplerCreateInfo.maxAnisotropy = 16;
            samplerCreateInfo.compareEnable = VK_FALSE;
            samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
            samplerCreateInfo.minLod = 0;
            samplerCreateInfo.maxLod = 0;
            samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
            samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

            vkCreateSampler(logicalHandle, &samplerCreateInfo, nullptr, &sampler);
        }

        void changeLayout(sp<Command> command, VkImageLayout newLayout) {
            auto logicalHandle = device->getLogicalHandle();


            VkCommandBufferAllocateInfo allocateInfo;
            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.pNext = nullptr;
            allocateInfo.commandPool = command->getCommandPool();
            allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocateInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(logicalHandle, &allocateInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo;
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.pNext = nullptr;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            beginInfo.pInheritanceInfo = nullptr;

            // record
            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            VkImageMemoryBarrier imageMemoryBarrier;
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.pNext = nullptr;
            if (vkImageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            } else if (vkImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            } else {
                // TODO: insert valid Exception
                throw 1;
            }
            imageMemoryBarrier.oldLayout = vkImageLayout;
            imageMemoryBarrier.newLayout = newLayout;
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.image = vkImage;
            imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

            vkCmdPipelineBarrier(commandBuffer,
                    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    0,
                    0,
                    nullptr,
                    0,
                    nullptr,
                    1,
                    &imageMemoryBarrier);

            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo;
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.pNext = nullptr;
            submitInfo.waitSemaphoreCount = 0;
            submitInfo.pWaitSemaphores = nullptr;
            submitInfo.pWaitDstStageMask = nullptr;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            submitInfo.signalSemaphoreCount = 0;
            submitInfo.pSignalSemaphores = nullptr;

            auto queue = device->getQueues()[0][0];
            vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(queue);

            vkFreeCommandBuffers(logicalHandle, command->getCommandPool(), 1, &commandBuffer);

            vkImageLayout = newLayout;
        }

        void writeBufferToImage(sp<Command> command) {
            auto logicalHandle = device->getLogicalHandle();

            VkCommandBufferAllocateInfo allocateInfo;
            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.pNext = nullptr;
            allocateInfo.commandPool = command->getCommandPool();
            allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocateInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(logicalHandle, &allocateInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo;
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.pNext = nullptr;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            beginInfo.pInheritanceInfo = nullptr;

            // record
            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            VkBufferImageCopy bufferImageCopy;
            bufferImageCopy.bufferOffset = 0;
            bufferImageCopy.bufferRowLength = 0;
            bufferImageCopy.bufferImageHeight = 0;
            bufferImageCopy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT,
                                                0,
                                                0,
                                                1};
            bufferImageCopy.imageOffset = {0, 0, 0};
            bufferImageCopy.imageExtent = {static_cast<uint32_t>(getWidth()),
                                           static_cast<uint32_t>(getHeight()),
                                           1};

            vkCmdCopyBufferToImage(commandBuffer,
                    buffer,
                    vkImage,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1,
                    &bufferImageCopy);

            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo;
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.pNext = nullptr;
            submitInfo.waitSemaphoreCount = 0;
            submitInfo.pWaitSemaphores = nullptr;
            submitInfo.pWaitDstStageMask = nullptr;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            submitInfo.signalSemaphoreCount = 0;
            submitInfo.pSignalSemaphores = nullptr;

            auto queue = device->getQueues()[0][0];
            vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(queue);

            vkFreeCommandBuffers(logicalHandle, command->getCommandPool(), 1, &commandBuffer);

        }

    private:
        int height;
        int width;
        int channels;
        stbi_uc* pixelData;
        VkImage vkImage;
        VkDeviceMemory memory;
        VkImageView vkImageView;
        VkImageLayout vkImageLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        VkSampler sampler;
        VkBuffer buffer;

        bool uploaded;
        sp<Device> device;
    };
}


#endif //GEOMETRYENGINE_IMAGE_H
