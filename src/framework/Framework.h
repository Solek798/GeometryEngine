//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_FRAMEWORK_H
#define GEOMETRYENGINE_FRAMEWORK_H

#include <Manageable.h>

namespace geo::framework {
    class Framework : Manageable {
    public:
        Framework();
        virtual ~Framework();

        void setup() override;
        void shutdown() override;

    private:
        bool inValidState;

        // Inner Classes
    };
}


#endif //GEOMETRYENGINE_FRAMEWORK_H
