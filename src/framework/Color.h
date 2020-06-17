//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_COLOR_H
#define GEOMETRYENGINE_COLOR_H

#include <cstdint>

namespace geo::framework {
    class Color {
    public:
        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color YELLOW;
        static const Color MAGENTA;
        static const Color CYAN;
        static const Color TRANSPARENT;

        Color();
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
        Color(const Color& other) = default;
        virtual ~Color() = default;

        Color& operator=(const Color& other);
        bool operator==(const Color& other) const;

        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;

    };
}


#endif //GEOMETRYENGINE_COLOR_H
