//
// Created by felix on 18.06.20.
//

#include "Detectable.h"
#include "Framework.h"

geo::framework::Detectable::Detectable() {
    Framework::get().registerDetectable(this);
}

geo::framework::Detectable::~Detectable() {
    Framework::get().unregisterDetectable(this);
}

void geo::framework::Detectable::markAsUpdated() {
    wasUpdated = true;
}
