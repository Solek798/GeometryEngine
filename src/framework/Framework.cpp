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

geo::framework::Framework::Framework()
    : inValidState(false)
    , run(false) { }

geo::framework::Framework::~Framework() {
    if (inValidState) {
        shutdown();
    }
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
