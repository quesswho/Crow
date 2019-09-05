#include "Game.h"

#include "glm/vec3.hpp"

using namespace Crow;

	Layer2D::Layer2D()
	{
		RenderAPI::ClearColor(0.5, 0.7, 0.5);

		m_Renderer = std::make_unique<Renderer2D>();

		BufferProperties bufferprop = { 
			{ 3, "a_Position" } 
		};

		ArrayBuffer* m_Buffer = new ArrayBuffer();

		const float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		const uint indices[] = {
			0,1,2
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), bufferprop);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint));


		m_Buffer->AddVertexBuffer(vertexBuffer);
		m_Buffer->SetIndexBuffer(indexBuffer);

		Shader* shader = new Shader("Basic", "res/Shader/Basic.glsl");
		shader->Bind();

		ShaderManager::PushShader(shader);

		m_Object = new Object2D(m_Buffer, shader);
	}

	Layer2D::~Layer2D()
	{
	}

	void Layer2D::OnEvent(Event& e)
	{
		auto [key, action] = e.getKeyValue();
		switch (e.m_Type)
		{
		case MOUSEPOS: // key = x, action = y
			//CR_GAME_INFO("Mouse Pos: {}, {}", key, action);
			break;
		case KEY:
			CR_GAME_INFO("Keyboard: {}", key);
			if (key == CROW_KEY_ESCAPE && action == CROW_KEY_PRESS)
				Crow::Application::Shutdown();

			m_Color = glm::vec3(0.0f, 0.0f, 0.0f);
			if (Input::IsKeyPressed(CROW_KEY_R))
			{
				m_Color = glm::vec3(0.9f, m_Color.y, m_Color.z);
			}
			if (Input::IsKeyPressed(CROW_KEY_G))
			{
				m_Color = glm::vec3(m_Color.x, 0.9f, m_Color.z);
			}
			if (Input::IsKeyPressed(CROW_KEY_B))
			{
				m_Color = glm::vec3(m_Color.x, m_Color.y, 0.9f);
			}

			break;
		case MOUSE:
			CR_GAME_INFO("Mouse Button: {}", key);
			break;
		}
	}

	void Layer2D::OnRender()
	{
		ShaderManager::GetShader("Basic")->SetUniform3f("color", m_Color);
		m_Renderer->Submit(m_Object);
		m_Renderer->Flush();
	}

class Game : public Crow::Application {
	
public:
	Game()
	{
		PushLayer(new Layer2D());
	}

	~Game()
	{}

};

void main()
{
	Game* game = new Game();
	game->Run();
	delete game;
}