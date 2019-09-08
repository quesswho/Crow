#pragma once

#include "Crow.h"


class Layer2D : public Crow::Layer {
public:

	Layer2D();
	~Layer2D();

	void OnEvent(Crow::Event& e) override;
	void OnUpdate(double elapsed) override;
	void OnRender() override;


	std::unique_ptr<Crow::Renderer2D> m_Renderer;
private:
	std::vector<Crow::Object2D*> m_Objects;
	Crow::OrthographicCamera* m_Camera;
};