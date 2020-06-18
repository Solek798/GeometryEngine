//
// Created by felix on 18.06.20.
//

#ifndef GEOMETRYENGINE_TESTAPP_H
#define GEOMETRYENGINE_TESTAPP_H

#include "framework/GeometryApplication.h"

class TestApp : public geo::framework::GeometryApplication {
public:
    TestApp();
    virtual ~TestApp() = default;

    void onStart() override;
    void onUpdate() override;
    void onStop() override;

private:
    float counter;
};


#endif //GEOMETRYENGINE_TESTAPP_H
