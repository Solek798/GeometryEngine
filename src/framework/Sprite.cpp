//
// Created by felix on 17.06.20.
//

#include "Sprite.h"


geo::framework::Sprite::Sprite()
    : transform(Transform())
    , albedo(Color())
    , texture(nullptr) { }

geo::framework::Sprite::Sprite(const geo::framework::Sprite &other)
    : transform(other.transform)
    , albedo(other.albedo)
    , texture(other.texture) { }

geo::framework::Sprite::Sprite(sp<geo::framework::Image> texture)
    : transform(Transform())
    , albedo(Color())
    , texture(texture) { }

geo::framework::Sprite::Sprite(const geo::framework::Color &albedo)
    : transform(Transform())
    , albedo(albedo)
    , texture(nullptr) { }

geo::framework::Sprite::Sprite(const geo::framework::Transform &transform)
    : transform(transform)
    , albedo(Color())
    , texture(nullptr) { }

geo::framework::Sprite::Sprite(sp<geo::framework::Image> texture, const geo::framework::Color &albedo)
    : transform(Transform())
    , albedo(albedo)
    , texture(texture) { }

geo::framework::Sprite::Sprite(sp<geo::framework::Image> texture, const geo::framework::Transform &transform)
    : transform(transform)
    , albedo(Color())
    , texture(texture) { }

geo::framework::Sprite::Sprite(const geo::framework::Color &albedo, const geo::framework::Transform &transform)
    : transform(transform)
    , albedo(albedo)
    , texture(nullptr) { }

geo::framework::Sprite::Sprite(sp<geo::framework::Image> texture, const geo::framework::Color &albedo,
                               const geo::framework::Transform &transform)
                               : transform(transform)
                               , albedo(albedo)
                               , texture(texture) { }

geo::framework::Sprite::Sprite(sp<geo::framework::Image> texture, const geo::framework::Color &albedo,
                               const geo::framework::Vector2 &position, float rotation)
                               : transform(Transform(position, rotation))
                               , albedo(albedo)
                               , texture(texture) { }

geo::framework::Sprite::Sprite(sp<geo::framework::Image> texture, const geo::framework::Color &albedo, int32_t posX,
                               int32_t posY, float rotation)
                               : transform(Transform(posX, posY, rotation))
                               , albedo(albedo)
                               , texture(texture) { }


geo::framework::Sprite &geo::framework::Sprite::operator=(const geo::framework::Sprite &other) {

    if (&other != this) {
        this->transform = other.transform;
        this->albedo = other.albedo;
        this->texture = other.texture;
    }

    return *this;
}

bool geo::framework::Sprite::operator==(const geo::framework::Sprite &other) const {
    return (this->transform == other.transform) && (this->albedo == other.albedo) && (this->texture == other.texture);
}


const geo::framework::Transform &geo::framework::Sprite::getTransform() const {
    return transform;
}

void geo::framework::Sprite::setTransform(const geo::framework::Transform &transform) {
    this->transform = transform;
}

const geo::framework::Color &geo::framework::Sprite::getAlbedo() const {
    return albedo;
}

void geo::framework::Sprite::setAlbedo(const geo::framework::Color &albedo) {
    this->albedo = albedo;
}

sp<geo::framework::Image> geo::framework::Sprite::getTexture() const {
    return texture;
}

void geo::framework::Sprite::setTexture(sp<geo::framework::Image> texture) {
    this->texture = texture;
}

void geo::framework::Sprite::move(int32_t x, int32_t y) {
    transform.translate(x, y);
}

void geo::framework::Sprite::move(const geo::framework::Vector2 &offset) {
    move(offset.getX(), offset.getY());
}

void geo::framework::Sprite::rotate(float degrees) {
    transform.rotate(degrees);
}

bool geo::framework::Sprite::overlaps(const geo::framework::Sprite &other) {
    // TODO: insert when image class is ready
    return false;
}


