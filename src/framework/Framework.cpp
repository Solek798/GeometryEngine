//
// Created by felix on 17.06.20.
//

#include "Framework.h"

// sleep methods for test purpose
#ifdef _WIN32
#include <windows.h>
#define SLEEP(time) Sleep(time);
#else
#include <unistd.h>
#define SLEEP(time) usleep(time * 1000);
#endif
#include <iostream>

up<geo::framework::Framework> geo::framework::Framework::instance = nullptr;

geo::framework::Framework &geo::framework::Framework::get() {
    return *instance;
}

geo::framework::Framework::Framework()
    : inValidState(false)
    , run(false) {
    if (!instance)
        instance = up<Framework>(this);
}

geo::framework::Framework::~Framework() {
    if (inValidState) {
        shutdown();
    }
    /*if (instance.get() == this) {
        instance.release();
    }*/
}

void geo::framework::Framework::setup() {
    inValidState = true;

    // Setup ...

    run = true;
}

void geo::framework::Framework::shutdown() {

    // Shutdown ...

    inValidState = false;
}

bool geo::framework::Framework::updateAndDraw() {
    std::cout << "update and draw :)" << std::endl;
    SLEEP(1000)
    return run;
}

void geo::framework::Framework::stop() {
    run = false;
}

void geo::framework::Framework::registerDetectable(geo::framework::Detectable* detectable) {
    //detectables.emplace_back(detectable);
}

void geo::framework::Framework::unregisterDetectable(geo::framework::Detectable* detectable) {
    //std::remove(detectables.front(), detectables.back(), detectable);
}


