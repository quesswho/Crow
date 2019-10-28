#pragma once

#include "Crow.h"

#include <memory>

struct Light {
	Light(glm::vec2 pos, glm::vec4 color)
		: m_Pos(pos), m_Color(color)
	{}

	glm::vec4 m_Color;
	glm::vec2 m_Pos;
};

class Layer2D : public Crow::Layer {
public:

	Layer2D();
	~Layer2D();

	void OnEvent(Crow::Event& e) override;
	void OnUpdate(float elapsed) override;
	void OnRender() override;


	std::unique_ptr<Crow::Renderer2D> m_Renderer;
private:
	std::vector<Crow::Object2D*> m_Objects;
	Crow::Object2D* m_Player;
	std::vector<Crow::Batchable2D*> m_BatchingObjects;
	Crow::OrthographicCamera* m_Camera;

	Crow::ArrayBuffer* m_ArrayBuffer;
	Crow::Shader* m_Shader;

	Light* m_Light;
};