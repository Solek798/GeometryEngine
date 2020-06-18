//
// Created by felix on 17.06.20.
//

#include "Framework.h"

geo::framework::Framework::Framework() : inValidState(false) {

}

geo::framework::Framework::~Framework() {
    if (inValidState) {
        shutdown();
    }
}

void geo::framework::Framework::setup() {
    inValidState = true;

    // Setup ...

}

void geo::framework::Framework::shutdown() {

    // Shutdown ...

    inValidState = false;
}
