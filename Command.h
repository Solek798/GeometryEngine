//
// Created by felix on 09.06.20.
//

#ifndef GEOMETRYENGINE_COMMAND_H
#define GEOMETRYENGINE_COMMAND_H


#include "Manageable.h"
#include "DeviceManager.h"
#include "Pipeline.h"

namespace geo {
    class Command : public Manageable {
    public:
        Command(sp<DeviceManager> deviceManager, sp<Pipeline> pipeline, std::vector<VkFramebuffer>& frameBuffer);
        virtual ~Command() = default;

        void setup() override;
        void shutdown() override;

        void record();

    private:
        VkCommandPoolCreateInfo poolCreateInfo;
        VkCommandPool commandPool;
        VkCommandBufferAllocateInfo allocateInfo;
        std::vector<VkCommandBuffer> commandBuffers;
        VkCommandBufferBeginInfo beginInfo;
        VkRenderPassBeginInfo passBeginInfo;

        sp<DeviceManager> deviceManager;
        sp<Pipeline> pipeline;
        std::vector<VkFramebuffer>& frameBuffer;
    };
}


#endif //GEOMETRYENGINE_COMMAND_H
