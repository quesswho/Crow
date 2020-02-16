#pragma once

#include "Crow.h"
#include "Crow/Graphics/Renderer/Buffer.h"
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
	std::vector<Crow::Batchable2D*> m_BatchingObjects;
	Crow::FirstPersonCamera* m_Camera;

	Crow::ArrayBuffer* m_ArrayBuffer;
	Crow::Shader* m_Shader;
	Crow::Texture* m_Texture;
	
	Crow::Font* m_OpenSansFont;

	Crow::PostEffect* m_PostFX;
	Crow::Shader* m_PostFXShader;
	Crow::Math::Mat4 m_Model;

	float m_Gamma;

	Light* m_Light;

	std::shared_ptr<Crow::IndexBuffer> m_IBuffer;
	std::shared_ptr<Crow::VertexBuffer> m_VBuffer;
};