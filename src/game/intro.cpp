#include "intro.h"

#include "game.h"

namespace {

using Point = engine::Point;
using engine::TILE_SIZE;
using Key = sf::Keyboard::Key;

void on_start(engine::GameWindow& game) {
    game.change_state(
        std::make_unique<game::MapState>()
    );
}

void on_aaaexit(engine::GameWindow& game) {
    game.close();
}

}

namespace game {

IntroState::IntroState()
{
    if (not bg_texture.loadFromFile("res/img/pattern.png")) {
        std::cout << "Failed to load bg pattern :(\n";
    }

    bg_texture.setRepeated(true);
    bg.setTexture(bg_texture);
    bg.setTextureRect({0, 0, 320, 256});
    bg.setPosition({-15, 0});

    if (not bgm.openFromFile("res/mus/genese4.ogg")) {
        std::cout << "Failed to load music :(\n";
    } else {
        bgm.setLoop(true);
        bgm.play();
    }

    if (not font.loadFromFile("res/font/PIXELADE.TTF")) {
        std::cout << "Failed to load font :(\n";
    }

    title = {"Prawrtect the City!", font, 48};
    title.setPosition(145, 30);
    title.setFillColor(COLORS[4]);
    title.setOutlineColor(COLORS[3]);
    title.setOutlineThickness(2);
    title.setOrigin(title.getGlobalBounds().width / 2, 0);

    options = {
        Option{sf::Text{"START", font, 24}, on_start},
        Option{sf::Text{"EXIT", font, 24}, on_aaaexit},
    };

    {
        auto i = 0u;
        for (auto& option: options) {
            option.text.setPosition(120, 100 + i * 32);
            option.text.setFillColor(COLORS[4]);
            option.text.setOutlineColor(COLORS[3]);
            option.text.setOutlineThickness(2);
            ++i;
        }
    }

    selection.setPoint(0, {0, 0});
    selection.setPoint(1, {16, 8});
    selection.setPoint(2, {0, 16});
    selection.setFillColor(COLORS[2]);
    selection.setOutlineColor(COLORS[3]);
    selection.setOutlineThickness(2);
    selection.setPosition(80, 112 + option * 32);
}


void IntroState::draw(sf::RenderWindow& window) {
    window.clear(COLORS[0]);
    window.draw(bg);

    window.draw(selection);

    window.draw(title);

    for (auto& option: options) {
        window.draw(option.text);
    }
}


void IntroState::update(engine::GameWindow& game) {
    options[option].text.setFillColor(COLORS[4]);

    if (keys[Key::Up]) {
        --option;
        keys[Key::Up] = false;
    } else if (keys[Key::Down]) {
        ++option;
        keys[Key::Down] = false;
    }

    if (keys[Keyboard::Key::Return]) {
        std::cout << "option: " << option << std::endl;
        bgm.stop();
        options[option].callback(game);
        return;
    }

    option %= options.size();

    selection.setPosition(90, 110 + option * 32);
    options[option].text.setFillColor(COLORS[2]);

    ++frame_count;
}

}
