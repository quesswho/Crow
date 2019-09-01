#pragma once
#include "Layer.h"
#include <vector>

namespace Crow {
	
	class LayerManager {
	public:
		LayerManager();
		~LayerManager();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		inline std::vector<Layer*>::iterator& begin() noexcept { return itBegin; }
		inline std::vector<Layer*>::iterator& end() noexcept { return itEnd; }
	private:
		std::vector<Layer*> m_Layers;

		std::vector<Layer*>::iterator itBegin;
		std::vector<Layer*>::iterator itEnd;

		inline void UpdateIterators() noexcept;
	};
}