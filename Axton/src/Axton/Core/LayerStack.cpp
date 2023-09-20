#include "axpch.h"
#include "LayerStack.h"

namespace Axton
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.insert(m_Layers.begin(), overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		m_Layers.pop_back();
		layer->OnDetach();
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		m_Layers.erase(m_Layers.begin());
		overlay->OnDetach();
	}
}