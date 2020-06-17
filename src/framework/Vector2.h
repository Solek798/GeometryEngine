//
// Created by felix on 17.06.20.
//

#ifndef GEOMETRYENGINE_VECTOR2_H
#define GEOMETRYENGINE_VECTOR2_H


namespace geo::framework {
    class Vector2 {
    public:
        Vector2();
        Vector2(float x, float y);
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
        void add(float x, float y);
        void sub(const Vector2& other);
        void sub(float x, float y);
        void mul(const Vector2& other);
        void mul(float x, float y);
        void div(const Vector2& other);
        void div(float x, float y);

        [[nodiscard]] float getX() const;
        void setX(float x);
        [[nodiscard]] float getY() const;
        void setY(float y);

        [[nodiscard]] float getLength() const;

    private:
        float x;
        float y;
    };
}


#endif //GEOMETRYENGINE_VECTOR2_H
