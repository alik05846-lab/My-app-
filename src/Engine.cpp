#include "native/Engine.hpp"

#include <algorithm>
#include <chrono>

namespace native
{
    bool Engine::initialize(Window& window, SoftwareRenderer& renderer, const WindowConfig& config)
    {
        if (running_) return false;
        if (!window.open(config)) return false;
        if (!renderer.initialize(window.width(), window.height()))
        {
            window.close();
            return false;
        }
        window_ = &window;
        renderer_ = &renderer;
        running_ = true;
        return true;
    }

    void Engine::run(const std::function<void(float)>& update,
                     const std::function<void(SoftwareRenderer&)>& render)
    {
        if (!running_ || !window_ || !renderer_) return;
        using clock = std::chrono::steady_clock;
        auto previous = clock::now();
        while (running_ && window_->is_open())
        {
            window_->poll_events();
            if (!window_->is_open()) break;
            const auto now = clock::now();
            const float delta = std::clamp(
                std::chrono::duration<float>(now - previous).count(), 0.0F, 0.1F);
            previous = now;
            renderer_->begin_frame();
            if (update) update(delta);
            if (render) render(*renderer_);
            renderer_->end_frame();
            input_.end_frame();
        }
    }

    void Engine::shutdown() noexcept
    {
        running_ = false;
        if (window_) window_->close();
        if (renderer_) renderer_->shutdown();
        window_ = nullptr;
        renderer_ = nullptr;
    }
}
