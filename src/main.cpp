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

using namespace geo::framework;

int main() {
    Transform transform{10, 10, 45};
    Color albedo{Color::MAGENTA};
    Sprite player(albedo, transform);

    player.move(-1, 1);


    auto position = player.getTransform().getPosition();

    std::cout << "Player position: " << position.getX() << ", " << position.getY() << std::endl;
    std::cout << "Player albedo: (" << static_cast<int>(player.getAlbedo().r) << ", " << player.getAlbedo().g << ", " <<player.getAlbedo().b << ", " << player.getAlbedo().a << ")" << std::endl;

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
