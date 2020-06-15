//
// Created by felix on 10.06.20.
//

#ifndef GEOMETRYENGINE_VERTEX_H
#define GEOMETRYENGINE_VERTEX_H

#include "GlobaleScope.h"
#include <glm/vec2.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace geo {
    class Vertex {
    public:
        static VkVertexInputBindingDescription getInputBindingDescription();
        static const sp<std::vector<VkVertexInputAttributeDescription>> getInputAttributeDescription();

        Vertex();
        Vertex(glm::vec2 position, glm::vec2 uv);
        Vertex(float x, float y, float u, float v);
        virtual ~Vertex() = default;



    private:
        glm::vec2 position;
        glm::vec2 uv;
    };
}


#endif //GEOMETRYENGINE_VERTEX_H
