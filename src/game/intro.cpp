#include "intro.h"

#include "game.h"

namespace {

using Point = engine::Point;
using engine::TILE_SIZE;

}

namespace game {

IntroState::IntroState()
{}


void IntroState::draw(sf::RenderWindow& window) {
    window.draw(bg);
}


void IntroState::update(engine::GameWindow& game) {
    if (keys[Keyboard::Key::Up]) {
        --option;
    } else if (keys[Keyboard::Key::Down]) {
        ++option;
    }

    if (keys[Keyboard::Key::C]) {
        on_confirm(game);
    }

    option %= max_options;

    ++frame_count;
}

void IntroState::on_confirm(engine::GameWindow& game) {
    switch (option) {
    case 0:
        game.change_state(
            std::make_unique<game::MapState>()
        );
        return;
    default:
        break;
    }
}

}
