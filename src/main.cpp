#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#include "Application.h"
#include <iostream>
#include <vector>
#include <memory>

#include "framework/Sprite.h"
#include "framework/Color.h"
#include "framework/Transform.h"
#include "TestApp.h"

using namespace geo::framework;

int main() {
    TestApp app;

    app.run();

    /*geo::Application app;

    app.setup();

#ifdef GEO_DEBUG_STATS
    app.getDeviceManager().getDevice(0)->debugPrintDeviceProperty();
#endif
#ifdef GEO_STATUS_NOTIFICATIONS
    std::cout << "#> Setup finished!" << std::endl;
#endif

    while(app.run());

    app.shutdown();*/


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
