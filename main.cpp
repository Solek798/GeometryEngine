#include <iostream>
//#include "Application.h"
#include "ECS/Component.h"

int main() {


    /*geo::Application app;

    app.setup();

    app.getDeviceManager().getDevice(0)->debugPrintDeviceProperty();

    while(app.run());

    app.shutdown();*/

    TestComponent test1{};
    TestComponent test2{};
    TestComponent test3{};

    TestComponent2 test11{};
    TestComponent2 test21{};
    TestComponent2 test31{};

    std::cout << "ID test 1: "<< test1.ID << std::endl;
    std::cout << "ID test 2: "<< test2.ID << std::endl;
    std::cout << "ID test 3: "<< test3.ID << std::endl;

    std::cout << "ID test 11: "<< test11.ID << std::endl;
    std::cout << "ID test 21: "<< test21.ID << std::endl;
    std::cout << "ID test 31: "<< test31.ID << std::endl;

    int i = 0;
    std::cout << "Hello, World! "<< ++i << std::endl;
    std::cout << "Hello, World! "<< i++ << std::endl;
    return 0;
}
