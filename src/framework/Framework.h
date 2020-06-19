//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_FRAMEWORK_H
#define GEOMETRYENGINE_FRAMEWORK_H

#include <Manageable.h>
#include "Detectable.h"
#include <vector>

namespace geo::framework {
class Framework : public geo::Manageable {
    public:
        // would be nicer if private but I couldn't bring it to work as friend Constructor for GeometryApplication
        Framework();
        virtual ~Framework();
        void setup() override;
        void shutdown() override;
        bool updateAndDraw();

        void stop();
        void registerDetectable(Detectable* detectable);
        void unregisterDetectable(Detectable* detectable);

        static Framework& get();

        // Debug method. Get's removed after testing is done
        //std::vector<Detectable*>& getDetectables() { return detectables; }
    private:
        static up<Framework> instance;

        bool inValidState;
        bool run; // Demo var to showcase ho Framework gets stopped. this is only one possible way
        // allocating this class with this vector inside throws a weird exception. Couldn't fix it.
        //std::vector<Detectable*> detectables;

        // Inner Classes
    };
}


#endif //GEOMETRYENGINE_FRAMEWORK_H
