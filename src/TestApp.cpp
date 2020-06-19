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

    sprite1 = new geo::framework::Sprite();
    sprite2 = new geo::framework::Sprite();

    //std::cout << "count: "<< geo::framework::Framework::get().getDetectables().size() << std::endl;
}

void TestApp::onUpdate() {
    std::cout << "Update: " << counter << std::endl;
    counter++;

    if (counter > 5)
        geo::framework::Framework::get().stop();
}

void TestApp::onStop() {
    std::cout << "<<<<<<< Stop" << std::endl;
    delete sprite1;
    delete sprite2;

    //std::cout << "count: "<< geo::framework::Framework::get().getDetectables().size() << std::endl;
}
