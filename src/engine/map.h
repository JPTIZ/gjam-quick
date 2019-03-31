#ifndef GJAM_QUICK_ENGINE_MAP_H
#define GJAM_QUICK_ENGINE_MAP_H

#include <set>

#include <SFML/Graphics.hpp>

#include "2d.h"
#include "utils.h"

namespace engine {

constexpr auto TILE_SIZE = 32;

const static auto passable_tiles = std::set{15, 16, 17};

template <typename T>
inline auto contains(const std::set<T>& s, T v) {
    return std::find(std::begin(s), std::end(s), v) != std::end(s);
}

template <size_t W, size_t H>
inline auto copy_map(const std::array<std::array<int, W>, H>& map) {
    auto vmap = std::vector<std::vector<int>>{};

    for (auto& row: map) {
        vmap.push_back({row.begin(), row.end()});
    }

    return vmap;
}

class Map: public sf::Sprite {
public:
    template <size_t H, size_t W>
    Map(const std::string& tileset,
        const std::array<std::array<int, W>, H>& mapping):
        sf::Sprite{},
        _size{int(W), int(H)},
        _map{copy_map(mapping)},
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

    auto passable(const Point& p) const {
        auto _p = p;
        _p = Point{
            _p.x / TILE_SIZE,
            _p.y / TILE_SIZE
        };
        return contains(passable_tiles, _map[_p.y][_p.x]) and
            contains(passable_tiles, _map[_p.y][_p.x + 1]);
    }

private:
    Size _size;
    std::vector<std::vector<int>> _map;
    sf::Image _tileset;
    sf::Image _map_image;
    sf::Texture _map_texture;
};

}

#endif
