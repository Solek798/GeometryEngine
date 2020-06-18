//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_TRANSFORM_H
#define GEOMETRYENGINE_TRANSFORM_H


#include "Vector2.h"

namespace geo::framework {
    struct Transform {
    public:
        Transform();
        Transform(const Vector2& position, float rotation);
        Transform(int32_t posX, int32_t posY, float rotation);
        Transform(const Transform& other) = default;
        virtual ~Transform() = default;

        Transform& operator=(const Transform& other);
        bool operator==(const Transform& other) const;

        [[nodiscard]] const Vector2& getPosition() const;
        void SetPosition(const Vector2& newPosition);
        [[nodiscard]] float getRotation() const;
        void setRotation(float newRotation);

        void translate(int32_t x, int32_t y);
        void translate(const Vector2& offset);
        void rotate(float degrees);

    private:
        Vector2 position;
        float rotation;
    };
}


#endif //GEOMETRYENGINE_TRANSFORM_H
