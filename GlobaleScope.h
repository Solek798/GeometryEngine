//
// Created by felix on 12.04.20.
//

#ifndef GEOMETRYENGINE_GLOBALESCOPE_H
#define GEOMETRYENGINE_GLOBALESCOPE_H

#include "vulkan/vulkan.h"
#include <memory>

#define EVER ;;
#define FULL_VIRTUAL 0

#define VK_ASSERT(result) \
if (result != VK_SUCCESS) { \
    throw 1; \
}

template <typename T> using sp = std::shared_ptr<T>;
template <typename T> using up = std::unique_ptr<T>;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

#endif //GEOMETRYENGINE_GLOBALESCOPE_H
