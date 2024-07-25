#pragma once
#include <Scene/Entity.h>

namespace Zero
{
	class EntitySelector
	{
	public:
		static Entity& GetEntitySelected() { return m_EntitySelected; }
		static void SetEntitySelected(Entity entity)
		{
			m_EntitySelected = entity;
		}

	private:
		static Entity m_EntitySelected;
	};
}