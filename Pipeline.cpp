//
// Created by felix on 08.06.20.
//

#include <utility>
#include <iostream>
#include "Pipeline.h"
#include "IO/FileInput.h"


geo::Pipeline::Pipeline(sp<DeviceManager> deviceManager) : deviceManager(std::move(deviceManager)) {

}

void geo::Pipeline::setup() {
    auto physicalHandle = deviceManager->getCurrentDevice()->getPhysicalHandle();
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    auto vertexData = FileInput::read_file(COMPILED_VERT);
    auto fragData = FileInput::read_file(COMPILED_FRAG);

    vertexModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertexModuleCreateInfo.pNext = nullptr;
    vertexModuleCreateInfo.flags = 0;
    vertexModuleCreateInfo.codeSize = vertexData->size();
    vertexModuleCreateInfo.pCode = reinterpret_cast<uint32_t*>(vertexData->data());
    vkCreateShaderModule(logicalHandle, &vertexModuleCreateInfo, nullptr, &vertexModule);

    fragModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragModuleCreateInfo.pNext = nullptr;
    fragModuleCreateInfo.flags = 0;
    fragModuleCreateInfo.codeSize = fragData->size();
    fragModuleCreateInfo.pCode = reinterpret_cast<uint32_t*>(fragData->data());
    vkCreateShaderModule(logicalHandle, &fragModuleCreateInfo, nullptr, &fragModule);

    vertexStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexStageCreateInfo.pNext = nullptr;
    vertexStageCreateInfo.flags = 0;
    vertexStageCreateInfo.pName = SHADER_START_METHOD;
    vertexStageCreateInfo.pSpecializationInfo = nullptr;
    vertexStageCreateInfo.module = vertexModule;
    vertexStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

    fragStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragStageCreateInfo.pNext = nullptr;
    fragStageCreateInfo.flags = 0;
    fragStageCreateInfo.pName = SHADER_START_METHOD;
    fragStageCreateInfo.pSpecializationInfo = nullptr;
    fragStageCreateInfo.module = fragModule;
    fragStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

    inputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    inputCreateInfo.pNext = nullptr;
    inputCreateInfo.flags = 0;
    inputCreateInfo.vertexBindingDescriptionCount = 0; // TODO does not work in real Application!
    inputCreateInfo.pVertexBindingDescriptions = nullptr; // TODO does not work in real Application!
    inputCreateInfo.vertexAttributeDescriptionCount = 0; // TODO does not work in real Application!
    inputCreateInfo.pVertexAttributeDescriptions = nullptr; // TODO does not work in real Application!

    inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.pNext = nullptr;
    inputAssemblyCreateInfo.flags = 0;
    inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = WINDOW_WIDTH;
    viewport.height = WINDOW_HEIGHT;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    scissor.offset = {0, 0};
    scissor.extent = {WINDOW_WIDTH, WINDOW_HEIGHT};

    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.pNext = nullptr;
    viewportStateCreateInfo.flags = 0;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;

    rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationStateCreateInfo.pNext = nullptr;
    rasterizationStateCreateInfo.flags = 0;
    rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
    rasterizationStateCreateInfo.lineWidth = 1.0f;

    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.pNext = nullptr;
    multisampleStateCreateInfo.flags = 0;
    multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // TODO: enable AA
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.minSampleShading = 1.0f;
    multisampleStateCreateInfo.pSampleMask = nullptr;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    colorBlendAttachmentState.blendEnable = VK_TRUE;
    colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.pNext = nullptr;
    colorBlendStateCreateInfo.flags = 0;
    colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_NO_OP;
    colorBlendStateCreateInfo.attachmentCount = 1;
    colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
    colorBlendStateCreateInfo.blendConstants[0] = VK_BLEND_FACTOR_CONSTANT_COLOR;
    colorBlendStateCreateInfo.blendConstants[1] = VK_BLEND_FACTOR_CONSTANT_COLOR;
    colorBlendStateCreateInfo.blendConstants[2] = VK_BLEND_FACTOR_CONSTANT_COLOR;
    colorBlendStateCreateInfo.blendConstants[3] = VK_BLEND_FACTOR_CONSTANT_COLOR;

    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutCreateInfo.pNext = nullptr;
    layoutCreateInfo.flags = 0;
    layoutCreateInfo.setLayoutCount = 0; // TODO: enable to be able to use uniforms in shader
    layoutCreateInfo.pSetLayouts = nullptr; // TODO: enable to be able to use uniforms in shader
    layoutCreateInfo.pushConstantRangeCount = 0; // TODO: enable to be able to use uniforms in shader
    layoutCreateInfo.pPushConstantRanges = nullptr; // TODO: enable to be able to use uniforms in shader
    vkCreatePipelineLayout(logicalHandle, &layoutCreateInfo, nullptr, &layout);

    // Start Attachments
    attachmentDescription.flags = 0;
    attachmentDescription.format = VK_FORMAT_B8G8R8A8_UNORM; //TODO: REALLY BAD INIT; should be the same as in SwapchainCreateInfo
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: Change if AA enabled
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // I am not sure what this is good for
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;

    attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentReference.attachment = 0;

    subpassDescription.flags = 0;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &attachmentReference;
    subpassDescription.pResolveAttachments = nullptr;
    subpassDescription.pDepthStencilAttachment = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;

    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = nullptr;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies = nullptr;

    vkCreateRenderPass(logicalHandle, &renderPassCreateInfo, nullptr, &renderPass);
    //End Attachments

    const std::vector<VkPipelineShaderStageCreateInfo> shaderStages{vertexStageCreateInfo, fragStageCreateInfo};

    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages.data();
    pipelineCreateInfo.pVertexInputState = &inputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    pipelineCreateInfo.pTessellationState = nullptr;
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineCreateInfo.pDepthStencilState = nullptr;
    pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    pipelineCreateInfo.pDynamicState = nullptr; // TODO: (?) set to enable Window resize
    pipelineCreateInfo.layout = layout;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = 0;
    vkCreateGraphicsPipelines(logicalHandle, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline);

#ifdef GEO_STATUS_NOTIFICATIONS
    std::cout << "#> Pipeline ready!" << std::endl;
#endif
}

void geo::Pipeline::shutdown() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    vkDestroyPipeline(logicalHandle, pipeline, nullptr);
    vkDestroyRenderPass(logicalHandle, renderPass, nullptr);
    vkDestroyPipelineLayout(logicalHandle, layout, nullptr);
    vkDestroyShaderModule(logicalHandle, vertexModule, nullptr);
    vkDestroyShaderModule(logicalHandle, fragModule, nullptr);
}

const VkRenderPass &geo::Pipeline::getRenderPass() const {
    return renderPass;
}

const VkPipeline &geo::Pipeline::getVulkanPipeline() const {
    return pipeline;
}
