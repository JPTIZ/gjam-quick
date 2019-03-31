#ifndef GJAM_QUICK_GAME_PLAYER_H
#define GJAM_QUICK_GAME_PLAYER_H

#include <SFML/Graphics.hpp>
#include "../engine/2d.h"
#include "../engine/sprite.h"

namespace game {

class GameObject {
public:
    GameObject() = default;

    GameObject(const std::filesystem::path& sprite):
        _sprite{sprite}
    {}

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
    engine::Point _pos{};
    engine::Sprite _sprite;
};

class Enemy: public GameObject {
public:
    Enemy():
        GameObject{"res/img/enemy.png"}
    {
        auto& _sprite = sprite();
        _sprite.max_frames() = 2;
        _sprite.animation(0);

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

    Enemy(Enemy&&) = default;

    Enemy(const Enemy&) = delete;

    auto being_hold() const {
        return _being_hold;
    }

    void being_hold(bool hold) {
        if (hold) {
            sprite().animation(1);
            sprite().max_frames() = 1;
            sprite().frame(0);
        } else {
            sprite().animation(2);
            _jump_time = 16;
        }
        _being_hold = hold;
    }

    auto jumping() {
        return _jump_time > 1;
    }

    auto& jump_time() {
        return _jump_time;
    }

    auto jump_speed() const {
        return 10;
    }

    auto destroyed() const {
        return _destroyed;
    }

    auto& destroyed() {
        return _destroyed;
    }

    auto speed() const {
        return _speed;
    }

    auto& speed() {
        return _speed;
    }

    auto exploding() const {
        return _exploding;
    }

    void explode() {
        sprite().animation(3);
        _exploding = true;
    }

private:
    bool _being_hold = false;
    bool _destroyed = false;
    bool _exploding = false;
    int _jump_time = 0;
    int _speed = 1;
};

class Player: public GameObject {
public:
    Player():
        GameObject{
            "res/img/dino.png"
        }
    {
        auto& _sprite = sprite();
        _sprite.max_frames() = 2;
        _sprite.animation(0);

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

    auto holding() const {
        return _holding;
    }

    void hold(Enemy* enemy) {
        std::cout << "HOLD!\n";
        _holding = enemy;
        sprite().animation(2);
        sprite().max_frames() = 1;
    }

    auto drop() {
        std::cout << "DROP!\n";
        auto object = _holding;
        _holding = nullptr;
        sprite().max_frames() = 2;
        return object;
    }

private:
    Enemy* _holding = nullptr;
};

}

#endif
