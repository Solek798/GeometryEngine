#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#undef STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "Application.h"
#include <vector>

#include <memory>

int main() {
    //std::shared_ptr<std::vector<int>> test = std::make_shared<std::vector<int>>(1);

  geo::Application app;

  app.setup();

#ifdef GEO_DEBUG_STATS
  app.getDeviceManager().getDevice(0)->debugPrintDeviceProperty();
#endif
#ifdef GEO_STATUS_NOTIFICATIONS
    std::cout << "#> Setup finished!" << std::endl;
#endif

  while(app.run());

  app.shutdown();


  std::cout << "Hello, World!" << std::endl;

    //geo::Image image{"../resources/rocket.png"};


  return 0;
}
