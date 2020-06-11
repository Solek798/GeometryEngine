//
// Created by felix on 10.06.20.
//

#ifndef GEOMETRYENGINE_VERTEX_H
#define GEOMETRYENGINE_VERTEX_H

#include <glm/vec2.hpp>
#include <vulkan/vulkan_core.h>

namespace geo {
    class Vertex {
    public:
        static VkVertexInputBindingDescription getInputBindingDescription();
        static VkVertexInputAttributeDescription getInputAttributeDescription();

        Vertex();
        explicit Vertex(glm::vec2 position);
        Vertex(float x, float y);
        virtual ~Vertex() = default;



    private:
        glm::vec2 position;
    };
}


#endif //GEOMETRYENGINE_VERTEX_H
