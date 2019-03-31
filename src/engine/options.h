#ifndef GJAM_QUICK_ENGINE_OPTIONS_H
#define GJAM_QUICK_ENGINE_OPTIONS_H

#include <string>

namespace options {
    using namespace std::string_literals;

    namespace window {
        constexpr auto WIDTH = 640;
        constexpr auto HEIGHT = 512;
    }

    namespace game {
        constexpr auto WIDTH = 320;
        constexpr auto HEIGHT = 256;
    }

    const auto TITLE = "Prawrtect the City!"s;
}

#endif
