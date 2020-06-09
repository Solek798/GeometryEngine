//
// Created by felix on 08.06.20.
//

#ifndef GEOMETRYENGINE_PIPELINE_H
#define GEOMETRYENGINE_PIPELINE_H

#include "Manageable.h"
#include "DeviceManager.h"

namespace geo {
    class Pipeline : public Manageable {
    public:
        explicit Pipeline(sp<DeviceManager> deviceManager);
        virtual ~Pipeline() = default;

        void setup() override;
        void shutdown() override;

    private:
        VkShaderModuleCreateInfo vertexModuleCreateInfo, fragModuleCreateInfo;
        VkShaderModule fragModule, vertexModule;
        VkPipelineShaderStageCreateInfo vertexStageCreateInfo, fragStageCreateInfo;
        VkPipelineVertexInputStateCreateInfo inputCreateInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
        VkPipelineLayoutCreateInfo layoutCreateInfo;
        VkPipelineLayout layout;
        VkAttachmentDescription attachmentDescription;
        VkAttachmentReference attachmentReference;

        sp<DeviceManager> deviceManager;
    };
}


#endif //GEOMETRYENGINE_PIPELINE_H
