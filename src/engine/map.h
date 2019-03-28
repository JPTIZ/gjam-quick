#ifndef GJAM_QUICK_ENGINE_MAP_H
#define GJAM_QUICK_ENGINE_MAP_H

#include <SFML/Graphics.hpp>

#include "2d.h"
#include "utils.h"

namespace engine {

constexpr auto TILE_SIZE = 30;

class Map: public sf::Sprite {
public:
    template <size_t H, size_t W>
    Map(const std::string& tileset,
        const std::array<std::array<int, W>, H>& mapping):
        sf::Sprite{},
        _size{int(W), int(H)},
        _tileset{from_file<sf::Image>(tileset)},
        _map_image{create_image(_size * TILE_SIZE)}
    {
        auto tileset_size = _tileset.getSize();

        for (auto y = 0u; y < H; ++y) {
            for (auto x = 0u; x < W; ++x) {
                auto tile_index = mapping[y][x];
                auto tile_x = int(tile_index % tileset_size.x);
                auto tile_y = int(tile_index / tileset_size.y);

                _map_image.copy(
                    _tileset,
                    x * TILE_SIZE, y * TILE_SIZE,
                    {
                        tile_x * TILE_SIZE, tile_y * TILE_SIZE,
                        TILE_SIZE, TILE_SIZE,
                    },
                    true
                );
            }
        }

        _map_texture.loadFromImage(_map_image);
        setTexture(_map_texture);
    }

private:
    Size _size;
    sf::Image _tileset;
    sf::Image _map_image;
    sf::Texture _map_texture;
};

}

#endif
