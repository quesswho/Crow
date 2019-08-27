#include "LayerManager.h"

namespace Crow {

	LayerManager::LayerManager()
	{}

	LayerManager::~LayerManager() {
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerManager::PushLayer(Layer* layer)
	{
		m_Layers.push_back(layer);
		UpdateIterators();
	}

	void LayerManager::PopLayer(Layer* layer)
	{
		const auto it = std::find(begin(), end(), layer);

		if (it != end())
		{
			m_Layers.erase(it);
			UpdateIterators();
		}
	}

	void LayerManager::UpdateIterators() noexcept
	{
		itBegin = m_Layers.begin();
		itEnd = m_Layers.end();
	}

}