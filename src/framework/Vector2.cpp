//
// Created by felix on 17.06.20.
//

#include "Vector2.h"
#include <math.h>

geo::framework::Vector2::Vector2()
    : x(0)
    , y(0) { }

geo::framework::Vector2::Vector2(int32_t x, int32_t y)
    : x(x)
    , y(y) { }

geo::framework::Vector2 &geo::framework::Vector2::operator=(const geo::framework::Vector2 &other) {

    if (&other != this) {
        this->x = other.x;
        this->y = other.y;
    }

    return *this;
}

geo::framework::Vector2 geo::framework::Vector2::operator+(const geo::framework::Vector2 &other) const {
    return Vector2 (this->x + other.x, this->y + other.x);
}

bool geo::framework::Vector2::operator==(const geo::framework::Vector2 other) const {
    return (this->x == other.x) && (this->y == other.y);
}


geo::framework::Vector2 geo::framework::Vector2::operator-(const geo::framework::Vector2 &other) const {
    return Vector2 (this->x - other.x, this->y - other.x);
}

geo::framework::Vector2 geo::framework::Vector2::operator*(const geo::framework::Vector2 &other) const {
    return Vector2 (this->x * other.x, this->y * other.x);
}

geo::framework::Vector2 geo::framework::Vector2::operator/(const geo::framework::Vector2 &other) const {
    return Vector2 (this->x / other.x, this->y / other.x);
}

geo::framework::Vector2 &geo::framework::Vector2::operator+=(const geo::framework::Vector2 &other) {
    add(other);
    return *this;
}

geo::framework::Vector2 &geo::framework::Vector2::operator-=(const geo::framework::Vector2 &other) {
    sub(other);
    return *this;
}

geo::framework::Vector2 &geo::framework::Vector2::operator*=(const geo::framework::Vector2 &other) {
    mul(other);
    return *this;
}

geo::framework::Vector2 &geo::framework::Vector2::operator/=(const geo::framework::Vector2 &other) {
    div(other);
    return *this;
}


void geo::framework::Vector2::add(const geo::framework::Vector2 &other) {
    add(other.x, other.y);
}
void geo::framework::Vector2::add(int32_t x, int32_t y) {
    this->x += x;
    this->y += y;
}

void geo::framework::Vector2::sub(const geo::framework::Vector2 &other) {
    sub(other.x, other.y);
}
void geo::framework::Vector2::sub(int32_t x, int32_t y) {
    this->x -= x;
    this->y -= y;
}

void geo::framework::Vector2::mul(const geo::framework::Vector2 &other) {
    mul(other.x, other.y);
}
void geo::framework::Vector2::mul(int32_t x, int32_t y) {
    this->x *= x;
    this->y *= y;
}

void geo::framework::Vector2::div(const geo::framework::Vector2 &other) {
    div(other.x, other.y);
}
void geo::framework::Vector2::div(int32_t x, int32_t y) {
    this->x /= x;
    this->y /= y;
}


uint32_t geo::framework::Vector2::getX() const {
    return x;
}
void geo::framework::Vector2::setX(int32_t x) {
    this->x = x;
}

uint32_t geo::framework::Vector2::getY() const {
    return y;
}
void geo::framework::Vector2::setY(int32_t y) {
    this->y = y;
}

float geo::framework::Vector2::getLength() const {
    return sqrtf(static_cast<float>(x * x + y * y));
}



