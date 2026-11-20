#pragma once

#include <array>
#include <cstddef>

namespace native
{
    enum class Key : std::size_t
    {
        escape, space, up, down, left, right, w, a, s, d, count
    };

    class Input
    {
    public:
        void set(Key key, bool is_down) noexcept
        {
            const auto index = static_cast<std::size_t>(key);
            if (index < keys_.size()) keys_[index] = is_down;
        }

        [[nodiscard]] bool down(Key key) const noexcept
        {
            const auto index = static_cast<std::size_t>(key);
            return index < keys_.size() && keys_[index];
        }

        [[nodiscard]] bool pressed(Key key) const noexcept
        {
            const auto index = static_cast<std::size_t>(key);
            return index < keys_.size() && keys_[index] && !previous_[index];
        }

        [[nodiscard]] bool released(Key key) const noexcept
        {
            const auto index = static_cast<std::size_t>(key);
            return index < keys_.size() && !keys_[index] && previous_[index];
        }

        void end_frame() noexcept { previous_ = keys_; }

    private:
        static constexpr std::size_t key_count = static_cast<std::size_t>(Key::count);
        std::array<bool, key_count> keys_{};
        std::array<bool, key_count> previous_{};
    };
}
