//
// Created by felix on 17.06.20.
//

#include "Color.h"

const geo::framework::Color geo::framework::Color::BLACK = geo::framework::Color(255, 255, 255, 255);
const geo::framework::Color geo::framework::Color::WHITE = geo::framework::Color(0, 0, 0, 255);
const geo::framework::Color geo::framework::Color::RED = geo::framework::Color(255, 0, 0, 255);
const geo::framework::Color geo::framework::Color::GREEN = geo::framework::Color(0, 255, 0, 255);
const geo::framework::Color geo::framework::Color::BLUE = geo::framework::Color(0, 0, 255, 255);
const geo::framework::Color geo::framework::Color::YELLOW = geo::framework::Color(255, 255, 0, 255);
const geo::framework::Color geo::framework::Color::MAGENTA = geo::framework::Color(255, 0, 255, 255);
const geo::framework::Color geo::framework::Color::CYAN = geo::framework::Color(0, 255, 255, 255);
const geo::framework::Color geo::framework::Color::TRANSPARENT = geo::framework::Color(255, 255, 255, 0);

geo::framework::Color::Color()
    : r(0)
    , g(0)
    , b(0)
    , a(0) { }

geo::framework::Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
    : r(r)
    , g(g)
    , b(b)
    , a(a) { }

geo::framework::Color &geo::framework::Color::operator=(const geo::framework::Color &other) {

    if (&other != this) {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        this->a = other.a;
    }

    return *this;
}

bool geo::framework::Color::operator==(const geo::framework::Color &other) const {
    return (this->r == other.r) && (this->g == other.g) && (this->b == other.b) && (this->a == other.a);
}
