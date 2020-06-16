//
// Created by felix on 09.06.20.
//

#include "Command.h"
#include "Vertex.h"

#include <utility>
#include <cstring>

geo::Command::Command(sp<DeviceManager> deviceManager, sp<Pipeline> pipeline, std::vector<VkFramebuffer>& frameBuffer, VkDescriptorSet* descriptorSet)
    : deviceManager(std::move(deviceManager))
    , pipeline(std::move(pipeline))
    , frameBuffer(frameBuffer) {
    this->descriptorSet = descriptorSet;
}

void geo::Command::setup() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.pNext = nullptr;
    poolCreateInfo.flags = 0;
    poolCreateInfo.queueFamilyIndex = 0; // TODO: BAD INIT
    vkCreateCommandPool(logicalHandle, &poolCreateInfo, nullptr, &commandPool);

    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = frameBuffer.size();

    commandBuffers.resize(frameBuffer.size());
    vkAllocateCommandBuffers(logicalHandle, &allocateInfo, commandBuffers.data());

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = sizeof(Vertex) * MAX_VERTEX_COUNT;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;
    vkCreateBuffer(logicalHandle, &bufferCreateInfo, nullptr, &vertexBuffer);

    vkGetBufferMemoryRequirements(logicalHandle, vertexBuffer, &memoryRequirements);

    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = deviceManager->getCurrentDevice()->findMemoryTypeIndex(
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(logicalHandle, &memoryAllocateInfo, nullptr, &memory);

    vkBindBufferMemory(logicalHandle, vertexBuffer, memory, 0);



    //record();
}

void geo::Command::shutdown() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    vkFreeMemory(logicalHandle, memory, nullptr);
    vkDestroyBuffer(logicalHandle, vertexBuffer, nullptr);
    vkFreeCommandBuffers(logicalHandle, commandPool, frameBuffer.size(), commandBuffers.data());
    vkDestroyCommandPool(logicalHandle, commandPool, nullptr);
}

void geo::Command::record() {
    for (int i=0 ; i<commandBuffers.size() ; i++) {
        vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

        passBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        passBeginInfo.pNext = nullptr;
        passBeginInfo.renderPass = pipeline->getRenderPass();
        passBeginInfo.framebuffer = frameBuffer[i];
        passBeginInfo.renderArea = {{0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT}};
        VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};
        passBeginInfo.clearValueCount = 1;
        passBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffers[i], &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVulkanPipeline());
        VkDeviceSize offsets[] {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &vertexBuffer, offsets);
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVulkanPipelineLayout(), 0, 1, descriptorSet, 0, nullptr);

        vkCmdDraw(commandBuffers[i], 4, 1, 0, 0); // TODO: Hardcoded vertex Count

        vkCmdEndRenderPass(commandBuffers[i]);
        vkEndCommandBuffer(commandBuffers[i]);
    }
}

const std::vector<VkCommandBuffer> &geo::Command::getCommandBuffers() const {
    return commandBuffers;
}

void geo::Command::mapMemory(const std::vector<Vertex>& vertecies) {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    void* data = nullptr;
    vkMapMemory(logicalHandle, memory, 0, bufferCreateInfo.size, 0, &data);

    memcpy(data, (void*)vertecies.data(), bufferCreateInfo.size);

    vkUnmapMemory(logicalHandle, memory);
}

const VkCommandPool &geo::Command::getCommandPool() const {
    return commandPool;
}
