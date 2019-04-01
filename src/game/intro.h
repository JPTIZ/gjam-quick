#ifndef GJAM_QUICK_GAME_INTRO_H
#define GJAM_QUICK_GAME_INTRO_H

#include <functional>
#include <set>

#include <SFML/Audio.hpp>

#include "../engine/engine.h"
#include "../engine/map.h"
#include "player.h"

namespace game {

struct Option {
    sf::Text text;
    std::function<void(engine::GameWindow&)> callback;
};

class IntroState: public engine::GameState {
    using Keyboard = sf::Keyboard;
    using Enemy = Player;
public:
    IntroState();

    void draw(sf::RenderWindow& window) override;

    void on_key_press(Keyboard::Key key) override {
        keys[key] = true;
    }

    void on_key_release(Keyboard::Key key) override {
        keys[key] = false;
    }

    void update(engine::GameWindow&) override;

private:
    int option = 0;
    int frame_count = 0;

    sf::Music bgm;

    sf::Sprite bg;
    sf::Texture bg_texture;

    sf::Font font;
    sf::Text title;

    sf::ConvexShape selection{3};

    std::vector<Option> options;

    std::map<Keyboard::Key, bool> keys;
};


}

#endif

