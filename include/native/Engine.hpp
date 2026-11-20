#pragma once

#include "native/Input.hpp"
#include "native/Renderer.hpp"
#include "native/Window.hpp"

#include <functional>

namespace native
{
    class Engine
    {
    public:
        bool initialize(Window& window, SoftwareRenderer& renderer, const WindowConfig& config = {});
        void run(const std::function<void(float)>& update,
                 const std::function<void(SoftwareRenderer&)>& render);
        void shutdown() noexcept;
        [[nodiscard]] bool running() const noexcept { return running_; }
        [[nodiscard]] Input& input() noexcept { return input_; }

    private:
        Window* window_ = nullptr;
        SoftwareRenderer* renderer_ = nullptr;
        Input input_;
        bool running_ = false;
    };
}
