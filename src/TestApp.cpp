//
// Created by felix on 18.06.20.
//

#include "TestApp.h"
#include <iostream>

TestApp::TestApp() {
    counter = 0;
}

void TestApp::onStart() {
    std::cout << "Start >>>>>>>" << std::endl;
}

void TestApp::onUpdate() {
    std::cout << "Update: " << counter << std::endl;
    counter++;

    if (counter > 5)
        getFramework().stop();
}

void TestApp::onStop() {
    std::cout << "<<<<<<< Stop" << std::endl;
}
