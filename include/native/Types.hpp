#pragma once

#include <cstdint>

namespace native
{
    struct Vec2
    {
        float x = 0.0F;
        float y = 0.0F;
    };

    struct Vec3
    {
        float x = 0.0F;
        float y = 0.0F;
        float z = 0.0F;
    };

    struct Transform
    {
        Vec3 position{};
        Vec3 rotation{};
        Vec3 scale{1.0F, 1.0F, 1.0F};
    };

    struct Color
    {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 255;

        friend constexpr bool operator==(Color, Color) = default;
    };

    struct Rect
    {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
    };
}
