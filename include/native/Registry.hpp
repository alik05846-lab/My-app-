#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace native
{
    struct Entity
    {
        std::uint32_t index = UINT32_MAX;
        std::uint32_t generation = 0;

        [[nodiscard]] constexpr bool valid() const noexcept
        {
            return index != UINT32_MAX;
        }

        friend constexpr bool operator==(Entity, Entity) = default;
    };

    struct EntityHash
    {
        [[nodiscard]] std::size_t operator()(Entity entity) const noexcept
        {
            return (static_cast<std::size_t>(entity.generation) << 32U) ^ entity.index;
        }
    };

    class Registry
    {
        struct IStorage
        {
            virtual ~IStorage() = default;
            virtual void erase(Entity entity) = 0;
        };

        template<class Component>
        struct Storage final : IStorage
        {
            std::unordered_map<Entity, Component, EntityHash> values;
            void erase(Entity entity) override { values.erase(entity); }
        };

    public:
        [[nodiscard]] Entity create()
        {
            std::uint32_t index;
            if (free_indices_.empty())
            {
                if (generations_.size() >= UINT32_MAX)
                    throw std::overflow_error("entity index space exhausted");
                index = static_cast<std::uint32_t>(generations_.size());
                generations_.push_back(1);
                alive_.push_back(true);
            }
            else
            {
                index = free_indices_.back();
                free_indices_.pop_back();
                alive_[index] = true;
            }
            ++alive_count_;
            return {index, generations_[index]};
        }

        [[nodiscard]] bool alive(Entity entity) const noexcept
        {
            return entity.valid() && entity.index < alive_.size() &&
                   alive_[entity.index] && generations_[entity.index] == entity.generation;
        }

        bool destroy(Entity entity)
        {
            if (!alive(entity)) return false;
            for (auto& [type, storage] : storages_)
            {
                (void)type;
                storage->erase(entity);
            }
            alive_[entity.index] = false;
            ++generations_[entity.index];
            free_indices_.push_back(entity.index);
            --alive_count_;
            return true;
        }

        [[nodiscard]] std::size_t size() const noexcept { return alive_count_; }

        template<class Component, class... Args>
        Component& emplace(Entity entity, Args&&... args)
        {
            require_alive(entity);
            auto& values = storage<Component>().values;
            auto [it, inserted] = values.try_emplace(entity, std::forward<Args>(args)...);
            if (!inserted) throw std::logic_error("component already exists on entity");
            return it->second;
        }

        template<class Component>
        [[nodiscard]] Component* get(Entity entity) noexcept
        {
            if (!alive(entity)) return nullptr;
            auto* values = find_storage<Component>();
            if (!values) return nullptr;
            auto it = values->values.find(entity);
            return it == values->values.end() ? nullptr : &it->second;
        }

        template<class Component>
        [[nodiscard]] const Component* get(Entity entity) const noexcept
        {
            if (!alive(entity)) return nullptr;
            const auto* values = find_storage<Component>();
            if (!values) return nullptr;
            auto it = values->values.find(entity);
            return it == values->values.end() ? nullptr : &it->second;
        }

        template<class Component>
        [[nodiscard]] bool has(Entity entity) const noexcept { return get<Component>(entity) != nullptr; }

        template<class Component>
        bool remove(Entity entity)
        {
            if (!alive(entity)) return false;
            auto* values = find_storage<Component>();
            return values && values->values.erase(entity) != 0;
        }

    private:
        void require_alive(Entity entity) const
        {
            if (!alive(entity)) throw std::invalid_argument("entity is not alive in this registry");
        }

        template<class Component>
        Storage<Component>& storage()
        {
            const std::type_index key(typeid(Component));
            auto it = storages_.find(key);
            if (it == storages_.end())
                it = storages_.emplace(key, std::make_unique<Storage<Component>>()).first;
            return *static_cast<Storage<Component>*>(it->second.get());
        }

        template<class Component>
        Storage<Component>* find_storage() noexcept
        {
            auto it = storages_.find(std::type_index(typeid(Component)));
            return it == storages_.end() ? nullptr : static_cast<Storage<Component>*>(it->second.get());
        }

        template<class Component>
        const Storage<Component>* find_storage() const noexcept
        {
            auto it = storages_.find(std::type_index(typeid(Component)));
            return it == storages_.end() ? nullptr : static_cast<const Storage<Component>*>(it->second.get());
        }

        std::vector<std::uint32_t> generations_;
        std::vector<bool> alive_;
        std::vector<std::uint32_t> free_indices_;
        std::unordered_map<std::type_index, std::unique_ptr<IStorage>> storages_;
        std::size_t alive_count_ = 0;
    };
}
