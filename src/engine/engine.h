#ifndef SUPERMAUROKART_ENGINE_H
#define SUPERMAUROKART_ENGINE_H

#include <SFML/Graphics.hpp>

namespace engine {

class GameState {
public:
    virtual ~GameState() {}

    virtual void draw(sf::RenderWindow&) = 0;
    virtual void update() = 0;
    virtual void on_key_press(sf::Keyboard::Key) = 0;
    virtual void on_key_release(sf::Keyboard::Key) = 0;
};

class GameWindow {
public:
    GameWindow(std::unique_ptr<GameState>&&);

    void show();
    void close();

private:
    void main_loop();
    void on_resize(const sf::Event::SizeEvent&);

    sf::RenderWindow window;
    std::unique_ptr<GameState> state = nullptr;
};

}

#endif
