#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <utility>

#include <entt/entt.hpp>

#include "../Scene.h"

namespace stardust
{
	class Entity
	{
	private:
		Scene* m_scene = nullptr;
		entt::entity m_handle = entt::null;

	public:
		Entity() = default;
		Entity(const entt::entity handle, Scene& scene);
		~Entity() noexcept = default;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_scene->GetEntityRegistry().emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template <typename T>
		void RemoveComponent() const
		{
			return m_scene->GetEntityRegistry().remove<T>(m_handle);
		}

		template <typename T>
		T& GetComponent() const
		{
			return m_scene->GetEntityRegistry().get<T>(m_handle);
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_scene->GetEntityRegistry().has<T>(m_handle);
		}

		entt::entity GetHandle() const noexcept { return m_handle; }

		bool operator ==(const Entity&) const = default;
		bool operator !=(const Entity&) const = default;

		inline operator bool() const noexcept { return m_handle != entt::null; }
		inline operator std::uint32_t() const noexcept { return static_cast<std::uint32_t>(m_handle); }
	};
}

#endif