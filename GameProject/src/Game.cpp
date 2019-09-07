#include "Game.h"

#include "glm/vec3.hpp"

using namespace Crow;

	Layer2D::Layer2D()
	{
		RenderAPI::ClearColor(0.5, 0.7, 0.5);

		m_Renderer = std::make_unique<Renderer2D>();

		BufferProperties bufferprop = { 
			{ 3 }, //vertices
			{ 2 }  //uvs
		};

		ArrayBuffer* m_Buffer = new ArrayBuffer();

		const float vertices[4*5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,
			0.5f,   0.5f, 0.0f, 1.0f, 1.0f
		};

		const uint indices[] = {
			0,1,2, 3,2,1
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), bufferprop);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint));


		m_Buffer->AddVertexBuffer(vertexBuffer);
		m_Buffer->SetIndexBuffer(indexBuffer);

		Texture* texture = new Texture("res/Texture/crow.png", TextureProperties(CROW_NEAREST_MIPMAP_NEAREST, CROW_NEAREST, CROW_CLAMP_TO_EDGE, CROW_CLAMP_TO_EDGE));

		Shader* shader = new Shader("Basic", "res/Shader/Basic.glsl");
		shader->Bind();
		shader->SetUniform1i("u_BasicTexture", texture->GetIndex());


		ShaderManager::PushShader(shader);
		CR_GAME_INFO(texture->GetWidth());
		m_Object = new Object2D(m_Buffer, shader, { texture });
	}

	Layer2D::~Layer2D()
	{
		delete m_Object;
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

			break;
		case MOUSE:
			CR_GAME_INFO("Mouse Button: {}", key);
			break;
		}
	}

	void Layer2D::OnRender()
	{
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