#include "Scene.h"

#include "../Application.h"
#include "entity/Entity.h"

namespace stardust
{
	Scene::Scene(Application& application, const std::string& name)
		: m_name(name), m_application(application), m_entityRegistry(application.GetEntityRegistry())
	{ }

	Entity Scene::CreateEntity()
	{
		return Entity(m_entityRegistry.create(), *this);
	}
}