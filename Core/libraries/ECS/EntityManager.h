#pragma once
#include <queue>
#include <unordered_map>
#include <ECS/Entity.h>
#include <Debug/Logger.h>

namespace Zero
{
	
	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager();
		Entity& CreateEntity();
		void DeleteEntity(Entity entity);
		void DeleteEntity(unsigned int id);
		Entity* GetEntity(unsigned int id);
		const inline std::unordered_map<unsigned int, Entity>& GetEntities() const { return m_Entities; }

	private:
		Logger m_Log;
		std::queue<unsigned int> m_FreeIds{};
		std::unordered_map<unsigned int, Entity> m_Entities;
	};
}
