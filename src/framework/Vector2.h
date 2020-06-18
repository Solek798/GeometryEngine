//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_VECTOR2_H
#define GEOMETRYENGINE_VECTOR2_H

#include <cstdint>

namespace geo::framework {
    struct Vector2 {
    public:
        Vector2();
        Vector2(int32_t x, int32_t y);
        Vector2(const Vector2& other) = default;
        virtual ~Vector2() = default;

        Vector2& operator=(const Vector2& other);
        bool operator==(const Vector2 other) const;
        Vector2 operator+(const Vector2& other) const;
        Vector2 operator-(const Vector2& other) const;
        Vector2 operator*(const Vector2& other) const;
        Vector2 operator/(const Vector2& other) const;
        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);
        Vector2& operator*=(const Vector2& other);
        Vector2& operator/=(const Vector2& other);

        void add(const Vector2& other);
        void add(int32_t x, int32_t y);
        void sub(const Vector2& other);
        void sub(int32_t x, int32_t y);
        void mul(const Vector2& other);
        void mul(int32_t x, int32_t y);
        void div(const Vector2& other);
        void div(int32_t x, int32_t y);

        [[nodiscard]] uint32_t getX() const;
        void setX(int32_t x);
        [[nodiscard]] uint32_t getY() const;
        void setY(int32_t y);

        [[nodiscard]] float getLength() const;

    private:
        int32_t x;
        int32_t y;
    };
}


#endif //GEOMETRYENGINE_VECTOR2_H
