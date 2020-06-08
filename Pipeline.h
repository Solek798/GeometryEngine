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
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;

        sp<DeviceManager> deviceManager;
    };
}


#endif //GEOMETRYENGINE_PIPELINE_H
