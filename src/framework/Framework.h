//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_FRAMEWORK_H
#define GEOMETRYENGINE_FRAMEWORK_H

#include <Manageable.h>

namespace geo::framework {
    class Framework : public Manageable {
    public:
        // would be nicer if private but I couldn't bring it to work as friend Constructor for GeometryApplication
        Framework();
        virtual ~Framework();
        void setup() override;
        void shutdown() override;


        void stop();

    private:
        friend class GeometryApplication;

        bool updateAndDraw();

        bool inValidState;
        bool run; // Demo var to showcase ho Framework gets stopped. this is only one possible way

        // Inner Classes
    };
}


#endif //GEOMETRYENGINE_FRAMEWORK_H
