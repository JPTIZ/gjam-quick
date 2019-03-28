#include "game.h"

constexpr auto PLAYER_SPEED = 5;

namespace {

using Point = engine::Point;
using engine::TILE_SIZE;

using Row = std::array<int, 25>;

const auto default_map = std::array<Row, 20>{
    Row
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, },
    {3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3, },
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, },
    {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, },
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, },
};

}

namespace game {

MapState::MapState():
    _map{
        "res/tileset.png",
        default_map
    }
{
    _map.setPosition({25, 0});
    auto& sprite = player.sprite();
    sprite.src_rect({{0, 0}, {34, 50}});
    sprite.max_frames() = 4;
    sprite.setOrigin({17.f, 25.f});
    player.move_to({400, 17 * 30});
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


void MapState::update() {
    update_enemies();

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
        _map.passable(player.pos() + player_offset - Point{45, 0})
    ) {
        player.move(player_offset);
        if (frame_count % 7 == 0) {
            player.sprite().next_frame();
        }
    } else {
        player.sprite().frame(0);
        return;
    }

    ++frame_count;
}


void MapState::update_enemies() {
    if (frame_count % 10 == 0) {
        // TODO: Show new enemy
    }
    for (auto& enemy: enemies) {
        // TODO: Update enemies behaviour
    }
}

}
