#include <iostream>
#include "Application.h"

int main() {
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
  return 0;
}
