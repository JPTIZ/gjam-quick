#ifndef GJAM_QUICK_GAME_GAME_H
#define GJAM_QUICK_GAME_GAME_H

#include <vector>

#include <SFML/Audio.hpp>

#include "../engine/engine.h"
#include "../engine/map.h"
#include "player.h"

namespace game {

const auto COLORS = std::vector<sf::Color>{
    // BG
    {179, 124, 202},

    // FG
    {112, 55, 135},

    // Selection
    {255, 255, 255},

    // Outline (Title)
    {0, 96, 0},

    // Fill (Title)
    {0, 164, 0},

    // Others
    {213, 97, 147},
    {255, 167, 204},
};

class MapState: public engine::GameState {
public:
    using Keyboard = sf::Keyboard;

    MapState();
    ~MapState() {
        for (auto enemy: enemies) {
            delete enemy;
        }
    }

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
    int spawn_count = 0;
    int last_spawn = 0;
    int points = 0;
    int lives = 5;
    bool paused = false;

    Player player;
    engine::Map _map;

    sf::Font font;
    sf::Text text_points{"0", font, 12};
    sf::Text text_points_label{"Points:", font, 12};
    sf::Text lives_label{"Lives:", font, 12};
    sf::Text lost{"GAME\nOVER", font, 64};
    sf::Text little_msg{"Press Enter to restart", font, 16};
    sf::Text paused_text{"Paused", font, 16};
    sf::Music bgm;

    std::map<Keyboard::Key, bool> keys;
    std::vector<Enemy*> enemies{};
    std::vector<Life> life_sprites{};
};


}

#endif
