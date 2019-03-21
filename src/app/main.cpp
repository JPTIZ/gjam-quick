#include <iostream>

#include "engine.h"
#include "game.h"

int main() {
    auto window = engine::GameWindow{
        std::make_unique<game::MapState>()
    };
    window.show();
}
