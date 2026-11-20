#pragma once

#include <cstdint>
#include <string>

namespace native
{
    struct WindowConfig
    {
        std::string title = "Native Engine";
        std::uint32_t width = 640;
        std::uint32_t height = 360;
    };

    class Window
    {
    public:
        virtual ~Window() = default;
        virtual bool open(const WindowConfig& config) = 0;
        virtual void poll_events() = 0;
        virtual void close() noexcept = 0;
        [[nodiscard]] virtual bool is_open() const noexcept = 0;
        [[nodiscard]] virtual std::uint32_t width() const noexcept = 0;
        [[nodiscard]] virtual std::uint32_t height() const noexcept = 0;
    };

    // Dependency-free backend for tests, CI, and headless demos.
    class HeadlessWindow final : public Window
    {
    public:
        explicit HeadlessWindow(std::uint32_t frame_limit = 3) noexcept
            : frame_limit_(frame_limit) {}
        bool open(const WindowConfig& config) override;
        void poll_events() override;
        void close() noexcept override;
        [[nodiscard]] bool is_open() const noexcept override { return open_; }
        [[nodiscard]] std::uint32_t width() const noexcept override { return width_; }
        [[nodiscard]] std::uint32_t height() const noexcept override { return height_; }
        [[nodiscard]] std::uint32_t frames_polled() const noexcept { return frames_polled_; }

    private:
        std::uint32_t frame_limit_;
        std::uint32_t frames_polled_ = 0;
        std::uint32_t width_ = 0;
        std::uint32_t height_ = 0;
        bool open_ = false;
    };
}
