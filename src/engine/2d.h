#ifndef GJAM_QUICK_ENGINE_2D_H
#define GJAM_QUICK_ENGINE_2D_H

namespace engine {

struct Point {
    int x;
    int y;

    auto operator-() const {
        return Point{-x, -y};
    }

    auto& operator+=(const Point& p) {
        *this = {x + p.x, y + p.y};
        return *this;
    }

    auto& operator-=(const Point& p) {
        *this += -p;
        return *this;
    }

    auto operator+(const Point& p) const {
        auto copy = *this;
        copy += p;
        return copy;
    }

    auto operator-(const Point& p) const {
        return *this + (-p);
    }
};

struct Size {
    int width;
    int height;

    auto& operator*=(int s) {
        *this = {width * s, height * s};
        return *this;
    }

    auto operator*(int s) const {
        auto copy = *this;
        copy *= s;
        return copy;
    }
};

struct Rect {
    Point pos;
    Size size;

    void move(const Point& dst) {
        pos = dst;
    }

    void resize(const Size& _size) {
        size = _size;
    }
};

}

#endif
