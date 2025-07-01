//
// Created by arunc on 01/07/2025.
//

#ifndef COLORUTILS_HPP
#define COLORUTILS_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <glm.hpp>
#include <stdexcept>

class ColorUtils {

public:
    static glm::vec3 HexToVec3(const std::string& hex) {
        std::string cleanHex = hex;
        if (hex[0] == '#') {
            cleanHex = hex.substr(1);
        }
        if (cleanHex.length() != 6)
            throw std::invalid_argument("Hex color must be 6 characters.");

        auto hexToFloat = [](const std::string& str) -> float {
            unsigned int value;
            std::stringstream ss;
            ss << std::hex << str;
            ss >> value;
            return static_cast<float>(value) / 255.0f;
        };

        float r = hexToFloat(cleanHex.substr(0, 2));
        float g = hexToFloat(cleanHex.substr(2, 2));
        float b = hexToFloat(cleanHex.substr(4, 2));

        return glm::vec3(r, g, b);
    }
};

#endif //COLORUTILS_HPP
