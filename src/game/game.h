#ifndef SUPERMAUROKART_GAME_H
#define SUPERMAUROKART_GAME_H

#include <set>

#include "engine/engine.h"
#include "engine/map.h"
#include "game/player.h"

namespace game {

class MapState: public engine::GameState {
    using Keyboard = sf::Keyboard;
    using Enemy = Player;
public:
    MapState();

    void draw(sf::RenderWindow& window) override;

    void on_key_press(Keyboard::Key key) override {
        keys[key] = true;
    }

    void on_key_release(Keyboard::Key key) override {
        keys[key] = false;
    }

    void update() override;

    void update_enemies();
private:
    int frame_count = 0;
    Player player;
    engine::Map _map;
    std::map<Keyboard::Key, bool> keys;
    std::set<Enemy> enemies;
};


}

#endif
