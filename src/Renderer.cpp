#include "native/Renderer.hpp"

#include <algorithm>
#include <limits>

namespace native
{
    bool SoftwareRenderer::initialize(std::uint32_t width, std::uint32_t height)
    {
        if (width == 0 || height == 0) return false;
        const auto count = static_cast<std::uint64_t>(width) * height;
        if (count > pixels_.max_size()) return false;
        try
        {
            pixels_.assign(static_cast<std::size_t>(count), Color{});
        }
        catch (...)
        {
            pixels_.clear();
            return false;
        }
        width_ = width;
        height_ = height;
        frames_rendered_ = 0;
        return true;
    }

    void SoftwareRenderer::shutdown() noexcept
    {
        pixels_.clear();
        width_ = height_ = 0;
        frames_rendered_ = 0;
    }

    void SoftwareRenderer::begin_frame(Color clear)
    {
        std::fill(pixels_.begin(), pixels_.end(), clear);
    }

    void SoftwareRenderer::draw_rect(Rect rect, Color color) noexcept
    {
        if (rect.width <= 0 || rect.height <= 0 || width_ == 0 || height_ == 0) return;
        const auto x0 = std::max<std::int64_t>(0, rect.x);
        const auto y0 = std::max<std::int64_t>(0, rect.y);
        const auto x1 = std::min<std::int64_t>(width_, static_cast<std::int64_t>(rect.x) + rect.width);
        const auto y1 = std::min<std::int64_t>(height_, static_cast<std::int64_t>(rect.y) + rect.height);
        for (auto y = y0; y < y1; ++y)
            for (auto x = x0; x < x1; ++x)
                pixels_[static_cast<std::size_t>(y) * width_ + static_cast<std::size_t>(x)] = color;
    }

    std::uint64_t SoftwareRenderer::checksum() const noexcept
    {
        std::uint64_t hash = 14695981039346656037ULL;
        for (const Color pixel : pixels_)
        {
            for (const std::uint8_t channel : {pixel.r, pixel.g, pixel.b, pixel.a})
            {
                hash ^= channel;
                hash *= 1099511628211ULL;
            }
        }
        return hash;
    }
}
