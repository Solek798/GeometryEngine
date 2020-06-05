//
// Created by felix on 19.05.20.
//

#ifndef GEOMETRYENGINE_MANAGEABLE_H
#define GEOMETRYENGINE_MANAGEABLE_H

#include "GlobaleScope.h"

namespace geo {
    class Manageable {
    public:
        virtual void setup() = FULL_VIRTUAL;
        virtual void shutdown() = FULL_VIRTUAL;
    };
}

#endif //GEOMETRYENGINE_MANAGEABLE_H
