#ifndef GJAM_QUICK_GAME_INTRO_H
#define GJAM_QUICK_GAME_INTRO_H

#include <set>

#include "../engine/engine.h"
#include "../engine/map.h"
#include "player.h"

namespace game {

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

    void on_confirm(engine::GameWindow&);
private:
    const int max_options = 2;
    int option = 0;
    int frame_count = 0;
    sf::Sprite bg;
    std::map<Keyboard::Key, bool> keys;
};


}

#endif

