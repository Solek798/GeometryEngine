//
// Created by felix on 17.06.20.
//

#include "Transform.h"

geo::framework::Transform::Transform()
    : position(Vector2())
    , rotation(0.0f) { }

geo::framework::Transform::Transform(const geo::framework::Vector2& position, float rotation)
    : position(position)
    , rotation(rotation) { }

geo::framework::Transform::Transform(float posX, float posY, float rotation)
    : position(Vector2(posX, posY))
    , rotation(rotation) { }


geo::framework::Transform &geo::framework::Transform::operator=(const geo::framework::Transform &other) {

    if (&other != this) {
        this->position = other.position;
        this->rotation = other.rotation;
    }

    return *this;
}

bool geo::framework::Transform::operator==(const geo::framework::Transform &other) const {
    return (this->position == other.position) && (this->rotation == other.rotation);
}


const geo::framework::Vector2 &geo::framework::Transform::getPosition() const {
    return position;
}
void geo::framework::Transform::SetPosition(const geo::framework::Vector2 &newPosition) {
    this->position = newPosition;
}

float geo::framework::Transform::getRotation() const {
    return rotation;
}
void geo::framework::Transform::setRotation(float newRotation) {
    this->rotation = newRotation;
}


void geo::framework::Transform::translate(float x, float y) {
    position.add(x, y);
}
void geo::framework::Transform::translate(const geo::framework::Vector2 &offset) {
    translate(offset.getX(), offset.getY());
}

void geo::framework::Transform::rotate(float degrees) {
    this->rotation += degrees;
}




