#include "game.h"

#include <iostream>
#include <optional>
#include <random>

#include "../engine/options.h"

constexpr auto PLAYER_SPEED = 5;

namespace {

constexpr auto MAP_SIZE = engine::Size{9, 8};

using Point = engine::Point;
using GameObject = game::GameObject;
using Player = game::Player;
using Enemy = game::Enemy;
using Key = sf::Keyboard::Key;
using engine::TILE_SIZE;

using Row = std::array<int, MAP_SIZE.width>;

const auto default_map = std::array<Row, MAP_SIZE.height>{
    Row
//   0  1  2  3  4  5  6  7  8
    {0, 1, 2, 1, 2, 1, 2, 1, 0,},
    {0, 3, 4, 3, 4, 3, 4, 3, 0,},
    {0, 5, 6, 5, 6, 5, 6, 5, 0,},
    {0, 7, 7, 7, 7, 7, 7, 7, 0,},
    {0, 8, 9, 8, 9, 8, 9, 8, 0,},
    {0,10,11,10,11,10,11,10, 0,},
    {0,12,13,13,13,13,13,14, 0,},
    {0,15,16,16,16,16,16,17, 0,},
};

void create_enemy(std::vector<Enemy*>& enemies, int enemy_speed) {
    static auto randomizer = std::mt19937{std::random_device{}()};
    auto distr = std::uniform_int_distribution<>{0, 5};

    auto lane = 2 + distr(randomizer);

    enemies.emplace_back(new Enemy{});

    auto& enemy = *enemies.back();
    enemy.move_to({lane * engine::TILE_SIZE, 10});
    {
        auto distr = std::uniform_int_distribution<>{1, enemy_speed};
        enemy.speed() = distr(randomizer);
    }
}

auto nearby(const game::GameObject* a, const game::GameObject* b, int near = 32) {
    auto [_a, _b] = std::pair{a->pos(), b->pos()};
    auto d = _b - _a;

    auto distance = std::sqrt(d.x * d.x + d.y * d.y);

    return distance < near;
}

auto grab_enemy(const Player& player, std::vector<Enemy*>& enemies) {
    for (auto& enemy: enemies) {
        if (nearby(&player, enemy)) {
            std::cout << "Found enemy nearby!\n";
            return enemy;
        }
    }
    return static_cast<Enemy*>(nullptr);
}

}


namespace game {

MapState::MapState():
    _map{
        "res/img/map_tileset.png",
        default_map
    }
{
    player.move_to({5 * 32, 7 * 32});
    if (not bgm.openFromFile("res/mus/nesssssss.ogg")) {
        std::cout << "Failed to load music :(\n";
        return;
    }

    bgm.setLoop(true);
    bgm.play();
}


void MapState::draw(sf::RenderWindow& window) {
    window.draw(_map);

    for (auto& enemy: enemies) {
        if (enemy->visible() and enemy->being_hold()) {
            window.draw(enemy->sprite());
        }
    }

    if (player.visible()) {
        window.draw(player.sprite());
    }

    for (auto& enemy: enemies) {
        if (enemy->visible() and not enemy->being_hold()) {
            window.draw(enemy->sprite());
        }
    }
}


void MapState::update(engine::GameWindow& w) {
    (void)w;
    static auto paused = false;

    if (paused) {
        if (keys[Key::Return]) {
            paused = false;
            bgm.play();

            keys[Key::Return] = false;
        }

        return;
    }

    if (keys[Key::Return]) {
        paused = true;
        bgm.pause();
        keys[Key::Return] = false;
        return;
    }

    update_enemies();
    update_player();

    ++frame_count;
}


void MapState::update_player() {
    auto player_offset = Point{0, 0};

    if (player.holding()) {
        if (keys[Key::Left]) {
            player_offset += {-PLAYER_SPEED, 0};
        } else if (keys[Key::Right]) {
            player_offset += {PLAYER_SPEED, 0};
        } else if (keys[Key::Space]) {
            auto enemy = player.drop();
            enemy->being_hold(false);

            keys[Key::Space] = false;
        }
    } else {
        if (keys[Key::Left]) {
            player.sprite().animation(1);
            player_offset += {-PLAYER_SPEED, 0};
        } else if (keys[Key::Right]) {
            player.sprite().animation(0);
            player_offset += {PLAYER_SPEED, 0};
        } else if (keys[Key::Space]) {
            auto enemy = grab_enemy(player, enemies);

            if (enemy) {
                player.hold(enemy);
                enemy->being_hold(true);
            }

            keys[Key::Space] = false;
        } else {
            player.sprite().frame(0);
        }
    }

    if (player_offset != Point{0, 0} and
        _map.passable(player.pos() + player_offset - Point{15, 0})
    ) {
        player.move(player_offset);
        if (frame_count % 7 == 0) {
            player.sprite().next_frame();
        }
    } else {
        player.sprite().frame(0);
        return;
    }
}


void MapState::update_enemies() {
    static auto spawn_count = 0;
    static auto last_spawn = 0;

    if (frame_count - last_spawn == spawn_delay) {
        create_enemy(enemies, spawn_enemy_speed);
        std::cout << "Enemy created! ("
            << "Spawn delay: " << spawn_delay << ","
            << "Speed: " << spawn_enemy_speed << ")\n";
        ++spawn_count;

        if (100 - spawn_delay == 0) {
            spawn_delay -= 3;
        }

        if (spawn_count % 30 == 0) {
            spawn_enemy_speed += 1;
        }

        last_spawn = frame_count;
    }

    auto new_vec = std::vector<Enemy*>{};

    for (auto& enemy: enemies) {
        if (enemy->jumping()) {
            enemy->move({0, -enemy->jump_speed()});
            --enemy->jump_time();

            for (auto& other: enemies) {
                if (enemy != other and nearby(enemy, other, 16)) {
                    std::cout << "Nearby enemies!\n";
                    enemy->destroyed() = true;
                    other->destroyed() = true;
                }
            }
        } else if (enemy->jump_time() == 1) {
            std::cout << "POW!\n";
            enemy->destroyed() = true;
            continue;
        } else if (not enemy->being_hold()) {
            enemy->move({0, enemy->speed()});
            if (frame_count % 15 == 0) {
                enemy->sprite().next_frame();
            }
        } else {
            enemy->move_to(player.pos() - Point{0, 15});
        }

        if (enemy->destroyed()) {
            delete enemy;
        } else if (enemy->pos().y < options::game::HEIGHT) {
            new_vec.push_back(std::move(enemy));
        }
    }

    enemies = std::move(new_vec);
}

}
