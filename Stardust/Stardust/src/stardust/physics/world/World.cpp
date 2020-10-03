#include "World.h"

namespace stardust
{
	namespace physics
	{
		World::World(const glm::vec2& gravity)
		{
			Initialise(gravity);
		}

		void World::Initialise(const glm::vec2& gravity)
		{
			m_handle = std::make_unique<b2World>(b2Vec2{ gravity.x, gravity.y });
		}

		void World::Step(const float timestep) const
		{
			m_handle->Step(timestep, static_cast<std::int32_t>(s_velocityIterations), static_cast<std::int32_t>(s_positionIterations));
		}
	}
}
