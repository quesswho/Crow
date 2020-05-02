#pragma once
#include "Crow.h"
#include <memory>

#define SCREENX 1080.0f
#define SCREENY 720.0f

class Noise : public Crow::Layer {
public:

	Noise();
	~Noise();

	void OnEvent(Crow::Event& e) override;
	void OnUpdate(float elapsed) override;
	void OnRender() override;

private:
	Crow::Font* m_OpenSansFont;

	Crow::PostEffect* m_PostFX;
	Crow::Shader* m_PostFXShader;

	std::shared_ptr<Crow::IndexBuffer> m_IBuffer;
	std::shared_ptr<Crow::VertexBuffer> m_VBuffer;
};