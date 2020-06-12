//
// Created by felix on 10.06.20.
//

#include "Vertex.h"

geo::Vertex::Vertex() : position(0, 0) { }

geo::Vertex::Vertex(glm::vec2 position) : position(position) { }

geo::Vertex::Vertex(float x, float y) : position(x, y) { }

VkVertexInputBindingDescription geo::Vertex::getInputBindingDescription() {
    VkVertexInputBindingDescription inputBindingDescription;
    inputBindingDescription.binding = 0;
    inputBindingDescription.stride = sizeof(Vertex);
    inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return inputBindingDescription;
}

VkVertexInputAttributeDescription geo::Vertex::getInputAttributeDescription() {
    VkVertexInputAttributeDescription inputAttributeDescription;
    inputAttributeDescription.location = 0;
    inputAttributeDescription.binding = 0;
    inputAttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
    inputAttributeDescription.offset = offsetof(Vertex, position);

    return inputAttributeDescription;
}
