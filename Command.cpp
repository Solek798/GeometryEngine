//
// Created by felix on 09.06.20.
//

#include "Command.h"

#include <utility>

geo::Command::Command(sp<DeviceManager> deviceManager, sp<Pipeline> pipeline, std::vector<VkFramebuffer>& frameBuffer)
    : deviceManager(std::move(deviceManager))
    , pipeline(std::move(pipeline))
    , frameBuffer(frameBuffer) {

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

    record();
}

void geo::Command::shutdown() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

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
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0); // TODO: Hardcoded vertex Count


        vkCmdEndRenderPass(commandBuffers[i]);
        vkEndCommandBuffer(commandBuffers[i]);
    }
}

const std::vector<VkCommandBuffer> &geo::Command::getCommandBuffers() const {
    return commandBuffers;
}
