#include <iostream>
#include "Application.h"

int main() {
  geo::Application app;

  app.setup();

#ifdef GEO_DEBUG
  app.getDeviceManager().getDevice(0)->debugPrintDeviceProperty();
#endif
  std::cout << "Application ready!" << std::endl;

  while(app.run());

  app.shutdown();

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
