#ifndef SUPERMAUROKART_GAME_H
#define SUPERMAUROKART_GAME_H

#include "engine.h"

namespace game {

class MapState: public engine::GameState {
    using Keyboard = sf::Keyboard;
public:
    MapState() {
        track.loadFromFile("res/battletrack1.png");
    }

    void draw(sf::RenderWindow& window) override {
        auto shape = sf::CircleShape{5.0f};

        shape.setFillColor({100, 250, 50});

        window.draw(shape);
    }

    void on_key_press(Keyboard::Key key) override {
        keys[key] = true;
    }

    void on_key_release(Keyboard::Key key) override {
        keys[key] = false;
    }

    void update() override {
        /*
        if (keys[Keyboard::Key::Up]) {
            --z;
        }
        */
    }

private:
    sf::Vector3f angle;
    float z;
    sf::Texture track;
    std::map<Keyboard::Key, bool> keys;
};


}

#endif
