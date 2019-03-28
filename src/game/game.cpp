#include "game.h"

constexpr auto PLAYER_SPEED = 5;

namespace game {

void MapState::draw(sf::RenderWindow& window) {
    window.draw(player.sprite());
}


void MapState::update() {
    static auto frame_count = 0;
    if (keys[Keyboard::Key::Left]) {
        player.sprite().setScale(1.f, 1.f);

        player.move({-PLAYER_SPEED, 0});
        if (frame_count % 10 == 0) {
            player.sprite().next_frame();
        }
    }
    if (keys[Keyboard::Key::Right]) {
        player.sprite().setScale(-1.f, 1.f);
        player.move({PLAYER_SPEED, 0});
        if (frame_count % 10 == 0) {
            player.sprite().next_frame();
        }
    }

    ++frame_count;
}

}
