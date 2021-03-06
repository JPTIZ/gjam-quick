#include "engine.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "2d.h"
#include "options.h"

using namespace sf;

namespace {

auto next_event(sf::RenderWindow& window) {
    auto event = sf::Event{};
    return window.pollEvent(event)
        ? std::optional{event}
        : std::nullopt;
}

}

// Members only!
namespace engine {

GameWindow::GameWindow(std::unique_ptr<GameState>&& initial_state):
    window{
        VideoMode{options::window::WIDTH, options::window::HEIGHT},
        options::TITLE,
    },
    state{std::move(initial_state)}
{
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setActive();
    window.setFramerateLimit(60);

    window.setView(sf::View{sf::FloatRect{
        -15.f, 0.f,
        static_cast<float>(options::game::WIDTH),
        static_cast<float>(options::game::HEIGHT)
    }});
}

void GameWindow::main_loop() {
    while (window.isOpen()) {
        while (auto _event = next_event(window)) {
            auto event = *_event;
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                if (state) {
                    state->on_key_press(event.key.code);
                }
            } else if (event.type == Event::KeyReleased) {
                if (state) {
                    state->on_key_release(event.key.code);
                }
            } else if (event.type == Event::Resized) {
                on_resize(event.size);
            }
        }

        if (state) {
            state->update(*this);
        }

        window.clear();

        if (state) {
            state->draw(window);
        }

        window.display();
    }
}

void GameWindow::on_resize(const Event::SizeEvent& size) {
    auto [width, height] = size;

    auto ratio = Size{
        static_cast<int>(width / options::game::WIDTH),
        static_cast<int>(height / options::game::HEIGHT),
    };

    if (ratio.width < 0 or ratio.height < 0) {
        window.setSize({options::game::WIDTH, options::game::HEIGHT});
        return;
    }

    /*
    window.setView(sf::View{{
        -(int(width) - ratio.width * options::game::WIDTH) / 2.f,
        0.f,
        static_cast<float>(ratio.width * options::game::WIDTH),
        static_cast<float>(options::game::HEIGHT)
    }});
    */
}

void GameWindow::show() {
    main_loop();
}

void GameWindow::close() {
    window.close();
}

}
