//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_SPRITE_H
#define GEOMETRYENGINE_SPRITE_H

#include "Transform.h"
#include "Image.h"
#include "GlobaleScope.h"
#include "Color.h"
#include "Detectable.h"

namespace geo::framework {
    class Sprite : public Detectable{
    public:
        Sprite();
        Sprite(const Sprite& other);
        explicit Sprite(sp<Image> texture);
        explicit Sprite(const Color& albedo);
        explicit Sprite(const Transform& transform);
        Sprite(sp<Image> texture, const Color& albedo);
        Sprite(sp<Image> texture, const Transform& transform);
        Sprite(const Color& albedo, const Transform& transform);
        Sprite(sp<Image> texture, const Color& albedo, const Transform& transform);
        Sprite(sp<Image> texture, const Color& albedo, const Vector2& position, float rotation);
        Sprite(sp<Image> texture, const Color& albedo, int32_t posX, int32_t posY, float rotation);
        virtual ~Sprite() = default;

        Sprite& operator=(const Sprite& other);
        bool operator==(const Sprite& other) const;

        [[nodiscard]] const Transform& getTransform() const;
        void setTransform(const Transform& transform);
        [[nodiscard]] const Color& getAlbedo() const;
        void setAlbedo(const Color& albedo);
        sp<Image> getTexture() const;
        void setTexture(sp<Image> texture);

        void move(int32_t x, int32_t y);
        void move(const Vector2& offset);
        void rotate(float degrees);
        bool overlaps(const Sprite& other);

    private:
        Transform transform;
        Color albedo;
        sp<Image> texture;
    };
}


#endif //GEOMETRYENGINE_SPRITE_H
