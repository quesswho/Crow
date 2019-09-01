#pragma once

#include "Crow.h"


class Layer2D : public Crow::Layer {
public:

	Layer2D();
	~Layer2D();

	void OnEvent(Crow::Event& e) override;
	void OnRender() override;

	Crow::Object2D* m_Object;

	std::unique_ptr<Crow::Renderer2D> m_Renderer;
};