#pragma once
#include <vector>
#include "Layer.h"
namespace Zero
{
	class LayerStack
	{
	public:
		LayerStack();
		virtual ~LayerStack();
		void pushLayer(Layer *);
		void popLayer(Layer *);

	private:
		std::vector<Layer*> m_Layers;
	};
}