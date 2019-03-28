#ifndef GJAM_QUICK_GAME_PLAYER_H
#define GJAM_QUICK_GAME_PLAYER_H

#include <SFML/Graphics.hpp>
#include "engine/2d.h"
#include "engine/sprite.h"

namespace game {

class Player {
public:
    Player() = default;

    const auto& pos() const {
        return _pos;
    }

    auto& pos() {
        return _pos;
    }

    auto x() const {
        return pos().x;
    }

    auto y() const {
        return pos().y;
    }

    void move(const engine::Point& offset) {
        _pos += offset;
        move_to(_pos);
    }

    void move_to(const engine::Point& dst) {
        _pos = dst;
        _sprite.setPosition(dst.x, dst.y);
    }

    const auto& sprite() const {
        return _sprite;
    }

    auto& sprite() {
        return _sprite;
    }

    auto visible() const {
        return _visible;
    }

    void visible(bool visible) {
        _visible = visible;
    }

private:
    bool _visible = true;
    engine::Point _pos;
    engine::Sprite _sprite{"res/bomberman.png"};
};

}

#endif
