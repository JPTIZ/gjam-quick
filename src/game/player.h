#ifndef GJAM_QUICK_GAME_PLAYER_H
#define GJAM_QUICK_GAME_PLAYER_H

#include <SFML/Graphics.hpp>
#include "../engine/2d.h"
#include "../engine/sprite.h"

namespace game {

class Player {
public:
    Player() {
        _sprite.max_frames() = 4;
        setup_sprite();
    }

    Player(const std::filesystem::path& sprite):
        _sprite{sprite}
    {
        _sprite.max_frames() = 4;
        setup_sprite();
    }

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

protected:
    virtual void setup_sprite() {
        auto sprite_size = _sprite.getTexture()->getSize();

        auto [frame_width, frame_height] = std::pair{
            int(sprite_size.x / 4),
            int(sprite_size.y),
        };

        _sprite.src_rect({
            {0, 0},
            {frame_width, frame_height}
        });

        sprite().setOrigin({
            static_cast<float>(frame_width / 2),
            static_cast<float>(frame_height / 2)
        });
    }

private:
    bool _visible = true;
    engine::Point _pos{};
    engine::Sprite _sprite{"res/bomberman.png"};
};

class Enemy: public Player {
public:
    Enemy():
        Player{"res/img/enemy.png"}
    {
        sprite().max_frames() = 2;
        sprite().animation(0);
        setup_sprite();
    }

    auto being_hold() const {
        return _being_hold;
    }

private:
    void setup_sprite() override {
        auto& _sprite = sprite();
        auto sprite_size = _sprite.getTexture()->getSize();

        auto [frame_width, frame_height] = std::pair{
            int(sprite_size.x / 2),
            int(sprite_size.y / 3),
        };

        _sprite.src_rect({
            {0, 0},
            {frame_width, frame_height}
        });

        _sprite.setOrigin({
            static_cast<float>(frame_width / 2),
            static_cast<float>(frame_height / 2)
        });
    }

    bool _being_hold = false;
};

}

#endif
