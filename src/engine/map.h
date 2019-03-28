#ifndef GJAM_QUICK_ENGINE_MAP_H
#define GJAM_QUICK_ENGINE_MAP_H

#include <SFML/Graphics.hpp>

#include "utils.h"

namespace engine {

class Map: public sf::Sprite {
public:
    Map(const std::string& tileset):
        sf::Sprite{}
    {
        _tileset = texture_from_file(tileset);
    }

private:
    sf::Texture _tileset;
};

}

#endif
