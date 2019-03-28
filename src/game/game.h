#ifndef SUPERMAUROKART_GAME_H
#define SUPERMAUROKART_GAME_H

#include "engine/engine.h"
#include "game/player.h"

namespace {



}

namespace game {

class MapState: public engine::GameState {
    using Keyboard = sf::Keyboard;
public:
    MapState() {
        track.loadFromFile("res/battletrack1.png");
        auto& sprite = player.sprite();
        sprite.src_rect({{0, 0}, {34, 50}});
        sprite.max_frames() = 4;
        sprite.setOrigin({17.f, 25.f});
        player.move_to({400, 400});
    }

    void draw(sf::RenderWindow& window) override;

    void on_key_press(Keyboard::Key key) override {
        keys[key] = true;
    }

    void on_key_release(Keyboard::Key key) override {
        keys[key] = false;
    }

    void update() override;
private:
    Player player;
    sf::Vector3f angle;
    sf::Texture track;
    std::map<Keyboard::Key, bool> keys;
};


}

#endif
