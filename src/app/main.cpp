#include <iostream>

#include "../engine/engine.h"
#include "../game/intro.h"

int main() {
    auto window = engine::GameWindow{
        std::make_unique<game::IntroState>()
    };
    window.show();
}
