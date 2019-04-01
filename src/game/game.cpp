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

static auto randomizer = std::mt19937{std::random_device{}()};

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

auto random_song() {
    const auto songs = std::array<std::string, 4>{
        "genese-lowsound.ogg",
        "genese3.ogg",
        "nesssssss.ogg",
        "amiguito.ogg",
    };
    auto distr = std::uniform_int_distribution<>{0, songs.size() - 1};

    return songs[distr(randomizer)];
}

void create_enemy(std::vector<Enemy*>& enemies, int enemy_speed) {
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

    if (not font.loadFromFile("res/font/PIXELADE.TTF")) {
        std::cout << "Failed to load font :(\n";
    } else {
        text_points.setFont(font);
        text_points_label.setFont(font);
    }

    text_points.setPosition({-8, 16});
    text_points_label.setPosition({-8, 4});
    text_points_label.setFillColor({192, 192, 0});

    lives_label.setPosition({8 + 8 * 32, 4});
    lives_label.setFillColor({192, 192, 0});

    auto song = random_song();

    std::cout << "Loaded " << song << " song" << std::endl;
    if (not bgm.openFromFile("res/mus/" + song)) {
        std::cout << "Failed to load music :(\n";
    } else {
        bgm.setLoop(true);
        bgm.play();
    }

    for (auto i = 0; i < lives; ++i) {
        auto life = Life{};

        auto& sprite = life.sprite();
        sprite.setPosition(8 + 8 * 32, 20 + 32 * i);

        life_sprites.push_back(std::move(life));
    }

    lost.setPosition(145, -10);
    lost.setFillColor(COLORS[0]);
    lost.setOutlineColor(COLORS[1]);
    lost.setOutlineThickness(2);
    lost.setOrigin(lost.getLocalBounds().width / 2, 0);

    little_msg.setPosition(145, 120);
    little_msg.setLetterSpacing(3.f);
    little_msg.setFillColor(COLORS[0]);
    little_msg.setOutlineColor(COLORS[1]);
    little_msg.setOutlineThickness(2);
    little_msg.setOrigin(little_msg.getLocalBounds().width / 2, 0);

    paused_text.setPosition(145, 120);
    paused_text.setLetterSpacing(3.f);
    paused_text.setFillColor(COLORS[0]);
    paused_text.setOutlineColor(COLORS[1]);
    paused_text.setOutlineThickness(2);
    paused_text.setOrigin(paused_text.getLocalBounds().width / 2, 0);
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

    window.draw(text_points_label);
    window.draw(text_points);
    window.draw(lives_label);

    for (auto& life: life_sprites) {
        window.draw(life.sprite());
    }

    if (lives == 0) {
        window.draw(lost);
        window.draw(little_msg);
    }

    if (paused) {
        window.draw(paused_text);
    }
}


void MapState::update(engine::GameWindow& w) {
    (void)w;

    if (lives > 0) {
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
    }

    update_enemies();

    if (lives > 0) {
        update_player();
    } else {
        if (keys[Key::Return]) {
            w.change_state(
                std::make_unique<game::MapState>()
            );
        }
    }

    for (auto& life: life_sprites) {
        if (frame_count % 5 == 0) {
            life.update();
        }
    }

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
    if (frame_count - last_spawn == spawn_delay) {
        create_enemy(enemies, spawn_enemy_speed);
        std::cout << "Enemy created! ("
            << "Spawn delay: " << spawn_delay << ", "
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
    auto streak = 0;

    constexpr auto POINTS_PER_STREAK = 5;

    for (auto& enemy: enemies) {
        if (enemy->jumping()) {
            enemy->move({0, -enemy->jump_speed()});
            --enemy->jump_time();

            for (auto& other: enemies) {
                if (enemy != other and nearby(enemy, other, 24)) {
                    std::cout << "Nearby enemies!\n";

                    ++streak;

                    enemy->destroyed() = true;
                    other->destroyed() = true;

                    points += POINTS_PER_STREAK * streak;
                }
            }

            text_points.setString(std::to_string(points));
        } else if (enemy->jump_time() == 1) {
            std::cout << "POW!\n";
            enemy->destroyed() = true;
            points += 1;

            text_points.setString(std::to_string(points));

            continue;
        } else if (not enemy->being_hold()) {
            enemy->move({0, enemy->speed()});
            if (frame_count % 15 == 0) {
                enemy->sprite().next_frame();
            }
        } else {
            enemy->move_to(player.pos() - Point{0, 20});
        }

        if (enemy->destroyed()) {
            delete enemy;
        } else if (enemy->pos().y < options::game::HEIGHT) {
            new_vec.push_back(std::move(enemy));
        } else {
            if (lives > 0) {
                --lives;
                life_sprites[lives].loose();

                std::cout << "Lost one life (" << lives << " remaining).\n";
            }

            if (lives == 0) {
                std::cout << "Lost!\n";
            }

            delete enemy;
        }
    }

    enemies = std::move(new_vec);
}

}
