//
// Created by felix on 09.06.20.
//

#ifndef GEOMETRYENGINE_COMMAND_H
#define GEOMETRYENGINE_COMMAND_H


#include "Manageable.h"
#include "DeviceManager.h"
#include "Pipeline.h"
#include "Vertex.h"

namespace geo {
    class Command : public Manageable {
    public:
        Command(sp<DeviceManager> deviceManager, sp<Pipeline> pipeline, std::vector<VkFramebuffer>& frameBuffer);
        virtual ~Command() = default;

        void setup() override;
        void shutdown() override;

        void record();
        void mapMemory(const std::vector<Vertex>& vertecies);

        [[nodiscard]] const std::vector<VkCommandBuffer>& getCommandBuffers() const;

    private:
        VkCommandPoolCreateInfo poolCreateInfo;
        VkCommandPool commandPool;
        VkCommandBufferAllocateInfo allocateInfo;
        std::vector<VkCommandBuffer> commandBuffers;
        VkCommandBufferBeginInfo beginInfo;
        VkRenderPassBeginInfo passBeginInfo;
        VkBufferCreateInfo bufferCreateInfo;
        VkBuffer vertexBuffer;
        VkMemoryRequirements memoryRequirements;
        VkMemoryAllocateInfo memoryAllocateInfo;
        VkDeviceMemory memory;

        sp<DeviceManager> deviceManager;
        sp<Pipeline> pipeline;
        std::vector<VkFramebuffer>& frameBuffer;
    };
}


#endif //GEOMETRYENGINE_COMMAND_H
