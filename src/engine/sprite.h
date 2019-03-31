#ifndef GJAM_QUICK_ENGINE_SPRITE_H
#define GJAM_QUICK_ENGINE_SPRITE_H

#include <exception>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "2d.h"
#include "utils.h"

namespace engine {

class Sprite: public sf::Sprite {
public:
    Sprite(const std::filesystem::path& texture):
        sf::Sprite{},
        _texture{texture_from_file(texture)}
    {
        setTexture(_texture);

        auto [width, height] = _texture.getSize();
        src_rect({{0, 0}, {int(width), int(height)}});
    }

    Sprite(Sprite&& s):
        sf::Sprite{std::move(s)},
        _frame{std::move(s._frame)},
        _max_frames{std::move(s._max_frames)},
        _src_rect{std::move(s._src_rect)},
        _texture{s._texture}
    {
        std::cout << "Moving sprite.\n";
    }

    Sprite(const Sprite& s):
        sf::Sprite{s},
        _frame{s._frame},
        _max_frames{s._max_frames},
        _src_rect{s._src_rect},
        _texture{s._texture}
    {
        std::cout << "Copying sprite.\n";
        setTexture(_texture);

        src_rect(s._src_rect);
    }

    auto& operator=(const Sprite& s) {
        if (this == &s) {
            return *this;
        }

        return *this;
    }

    auto frame() const {
        return _frame;
    }

    void next_frame() {
        frame((_frame + 1) % _max_frames);
    }

    void frame(int frame) {
        _frame = frame;
        update_texture_frame();
    }

    const auto animation() const {
        return _animation;
    }

    void animation(int animation) {
        _animation = animation;
        update_texture_frame();
    }

    auto max_frames() const {
        return _max_frames;
    }

    auto& max_frames() {
        return _max_frames;
    }

    const auto& src_rect() const {
        return _src_rect;
    }

    void src_rect(Rect src_rect) {
        _src_rect = src_rect;
        update_texture_frame();
    }

private:
    void update_texture_frame() {
        auto [x, y] = _src_rect.pos;
        auto [width, height] = _src_rect.size;

        setTextureRect({
            x + _frame * width,
            y + _animation * height,
            width,
            height,
        });
    }

    int _frame = 0;
    int _max_frames = 0;
    int _animation = 0;
    Rect _src_rect;
    sf::Texture& _texture;
};

}

#endif
