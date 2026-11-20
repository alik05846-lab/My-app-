#include "native/Window.hpp"

namespace native
{
    bool HeadlessWindow::open(const WindowConfig& config)
    {
        if (config.width == 0 || config.height == 0) return false;
        width_ = config.width;
        height_ = config.height;
        frames_polled_ = 0;
        open_ = true;
        return true;
    }

    void HeadlessWindow::poll_events()
    {
        if (!open_) return;
        ++frames_polled_;
        if (frame_limit_ != 0 && frames_polled_ >= frame_limit_) open_ = false;
    }

    void HeadlessWindow::close() noexcept { open_ = false; }
}
