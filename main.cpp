#include <iostream>
#include "Application.h"

int main() {
  geo::Application app;

  app.setup();

  app.getDeviceManager().getDevice(0)->debugPrintDeviceProperty();

  while(app.run());

  app.shutdown();

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
