//
// Created by felix on 10.06.20.
//

#include "Vertex.h"

geo::Vertex::Vertex() : position(0, 0) { }

geo::Vertex::Vertex(glm::vec2 position, glm::vec2 uv)
    : position(position)
    , uv(uv) { }

geo::Vertex::Vertex(float x, float y, float u, float v)
    : position(x, y)
    , uv(u, v) { }

VkVertexInputBindingDescription geo::Vertex::getInputBindingDescription() {
    VkVertexInputBindingDescription inputBindingDescription;
    inputBindingDescription.binding = 0;
    inputBindingDescription.stride = sizeof(Vertex);
    inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return inputBindingDescription;
}

const sp<std::vector<VkVertexInputAttributeDescription>> geo::Vertex::getInputAttributeDescription() {
    auto descriptions = std::make_shared<std::vector<VkVertexInputAttributeDescription>>(2);

    descriptions->at(0).location = 0;
    descriptions->at(0).binding = 0;
    descriptions->at(0).format = VK_FORMAT_R32G32_SFLOAT;
    descriptions->at(0).offset = offsetof(Vertex, position);

    descriptions->at(1).location = 1;
    descriptions->at(1).binding = 0;
    descriptions->at(1).format = VK_FORMAT_R32G32_SFLOAT;
    descriptions->at(1).offset = offsetof(Vertex, uv);

    return descriptions;
}
