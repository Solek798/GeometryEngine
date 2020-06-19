//
// Created by felix on 18.06.20.
//

#ifndef GEOMETRYENGINE_TESTAPP_H
#define GEOMETRYENGINE_TESTAPP_H

#include "framework/GeometryApplication.h"
#include "framework/Sprite.h"

class TestApp : public geo::framework::GeometryApplication {
public:
    TestApp();
    virtual ~TestApp() = default;

    void onStart() override;
    void onUpdate() override;
    void onStop() override;

private:
    float counter;

    geo::framework::Sprite *sprite1, *sprite2;
};


#endif //GEOMETRYENGINE_TESTAPP_H
