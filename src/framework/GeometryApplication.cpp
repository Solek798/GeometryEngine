//
// Created by felix on 17.06.20.
//

#include "GeometryApplication.h"

geo::framework::GeometryApplication::GeometryApplication()
    : framework(std::make_unique<Framework>()) { }

void geo::framework::GeometryApplication::run() {
    framework->setup();
    onStart();

    while (framework->updateAndDraw()) {
        onUpdate();
    }

    onStop();
    framework->shutdown();
}

geo::framework::Framework &geo::framework::GeometryApplication::getFramework() const {
    return *framework;
}
