#ifndef GJAM_QUICK_ENGINE_UTILS_H
#define GJAM_QUICK_ENGINE_UTILS_H


#include <SFML/Graphics.hpp>

#include <filesystem>
#include <string>

namespace engine {

sf::Texture texture_from_file(const std::filesystem::path& path);

}

#endif
