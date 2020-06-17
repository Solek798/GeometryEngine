//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_GEOMETRYAPPLICATION_H
#define GEOMETRYENGINE_GEOMETRYAPPLICATION_H


#include "Framework.h"

namespace geo::framework {
    class GeometryApplication {
    public:
        GeometryApplication();
        virtual ~GeometryApplication() = default;

        virtual void onStart() = 0;
        virtual void onUpdate() = 0;
        virtual void onStop() = 0;

    private:

    };
}


#endif //GEOMETRYENGINE_GEOMETRYAPPLICATION_H
