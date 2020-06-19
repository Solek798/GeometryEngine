//
// Created by felix on 17.06.20.
//

#include "GeometryApplication.h"

geo::framework::GeometryApplication::GeometryApplication() {
    // Create Singleton
    Framework();
}

void geo::framework::GeometryApplication::run() {
    auto framework = geo::framework::Framework::get();

    framework.setup();
    onStart();

    while (framework.updateAndDraw()) {
        onUpdate();
    }

    onStop();
    framework.shutdown();
}

geo::framework::GeometryApplication::~GeometryApplication() {
    Framework::get().~Framework();
}
