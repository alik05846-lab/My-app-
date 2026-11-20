#pragma once

#include "native/Types.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace native
{
    class SoftwareRenderer
    {
    public:
        bool initialize(std::uint32_t width, std::uint32_t height);
        void shutdown() noexcept;
        void begin_frame(Color clear = {0, 0, 0, 255});
        void draw_rect(Rect rect, Color color) noexcept;
        void end_frame() noexcept { ++frames_rendered_; }

        [[nodiscard]] std::uint32_t width() const noexcept { return width_; }
        [[nodiscard]] std::uint32_t height() const noexcept { return height_; }
        [[nodiscard]] std::uint64_t frames_rendered() const noexcept { return frames_rendered_; }
        [[nodiscard]] const std::vector<Color>& pixels() const noexcept { return pixels_; }
        [[nodiscard]] std::uint64_t checksum() const noexcept;

    private:
        std::uint32_t width_ = 0;
        std::uint32_t height_ = 0;
        std::uint64_t frames_rendered_ = 0;
        std::vector<Color> pixels_;
    };
}
