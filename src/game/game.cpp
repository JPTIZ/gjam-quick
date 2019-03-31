#include "game.h"

#include <iostream>
#include <random>

#include "../engine/options.h"

constexpr auto PLAYER_SPEED = 5;

namespace {

constexpr auto MAP_SIZE = engine::Size{11, 8};

using Point = engine::Point;
using engine::TILE_SIZE;

using Row = std::array<int, MAP_SIZE.width>;

const auto default_map = std::array<Row, MAP_SIZE.height>{
    Row
//   0  1  2  3  4  5  6  7  8  9 10
    {0, 0, 1, 2, 1, 2, 1, 2, 1, 0, 0, },
    {0, 0, 3, 4, 3, 4, 3, 4, 3, 0, 0, },
    {0, 0, 5, 6, 5, 6, 5, 6, 5, 0, 0, },
    {0, 0, 7, 7, 7, 7, 7, 7, 7, 0, 0, },
    {0, 0, 8, 9, 8, 9, 8, 9, 8, 0, 0, },
    {0, 0,10,11,10,11,10,11,10, 0, 0, },
    {0, 0,12,13,13,13,13,13,14, 0, 0, },
    {0, 0,15,16,16,16,16,16,17, 0, 0, },
};

void create_enemy(std::vector<game::Enemy>& enemies) {
    auto randomizer = std::mt19937{std::random_device{}()};
    auto distr = std::uniform_int_distribution<>{0, 5};

    auto lane = 2 + distr(randomizer);

    enemies.emplace_back();

    auto& enemy = enemies.back();
    enemy.move_to({lane * engine::TILE_SIZE, 10});
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
}


void MapState::draw(sf::RenderWindow& window) {
    window.draw(_map);

    if (player.visible()) {
        window.draw(player.sprite());
    }

    for (auto& enemy: enemies) {
        if (enemy.visible()) {
            window.draw(enemy.sprite());
        }
    }
}


void MapState::update(engine::GameWindow& w) {
    (void)w;

    update_enemies();
    update_player();

    ++frame_count;
}


void MapState::update_player() {
    auto player_speed = (keys[Keyboard::Key::X] ? 2 : 1) * PLAYER_SPEED;
    auto player_offset = Point{0, 0};

    if (keys[Keyboard::Key::Left]) {
        player.sprite().setScale(1.f, 1.f);
        player_offset += {-player_speed, 0};
    } else if (keys[Keyboard::Key::Right]) {
        player.sprite().setScale(-1.f, 1.f);
        player_offset += {player_speed, 0};
    } else {
        player.sprite().frame(0);
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
    if (frame_count % 100 == 0) {
        create_enemy(enemies);
    }

    auto new_vec = std::vector<Enemy>{};

    for (auto& enemy: enemies) {
        // TODO: Update enemies behaviour
        if (not enemy.being_hold()) {
            enemy.move({0, 1});
            if (frame_count % 15 == 0) {
                enemy.sprite().next_frame();
            }

            if (enemy.pos().y < options::game::HEIGHT) {
                new_vec.push_back(std::move(enemy));
            }
        }
    }

    enemies = std::move(new_vec);
}

}
