#ifndef GJAM_QUICK_GAME_GAME_H
#define GJAM_QUICK_GAME_GAME_H

#include <vector>

#include <SFML/Audio.hpp>

#include "../engine/engine.h"
#include "../engine/map.h"
#include "player.h"

namespace game {

class MapState: public engine::GameState {
public:
    using Keyboard = sf::Keyboard;

    MapState();

    void draw(sf::RenderWindow& window) override;

    void on_key_press(Keyboard::Key key) override {
        keys[key] = true;
    }

    void on_key_release(Keyboard::Key key) override {
        keys[key] = false;
    }

    void update(engine::GameWindow&) override;

    void update_enemies();
    void update_player();

private:
    int frame_count = 0;
    int spawn_delay = 60;
    int spawn_enemy_speed = 1;
    Player player;
    engine::Map _map;
    std::map<Keyboard::Key, bool> keys;
    std::vector<Enemy*> enemies{};
    sf::Music bgm;
};


}

#endif
