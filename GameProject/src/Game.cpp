#include "Game.h"

#include "glm/vec3.hpp"
#include "Crow/Graphics/ShaderFactory.h"

#include <DirectXMath.h>

using namespace Crow;

	Layer2D::Layer2D()
		: m_Camera(new OrthographicCamera(glm::vec2(0.0f, 0.0f), 2.0f, 2.0f, 3.0f, 3.0f))
	{
		Application::GetAPI()->ClearColor(0.5, 0.7, 0.5);
		Application::GetAPI()->EnableDepthTest();

		//m_Renderer = std::make_unique<Renderer2D>();

		BufferProperties bufferprop = { 
			{ "POSITION", 3 },//, //vertices
			{ "COLOR", 4 }  //uvs
		};

		//ArrayBuffer* m_Buffer = ArrayBuffer::Create();

		float vertices[] = {
			-0.5f,  0.5f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f,

			 // Further away (green)
			-0.75f, 0.75f, 0.2f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.0f,  0.2f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.75f, 0.0f,  0.2f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.75f, 0.2f, 0.0f, 1.0f, 0.0f, 1.0f
		};

		ulong indices[] {
			0,1,2, 0,3,1,
			4,5,6, 4,7,5
		};

		m_Shader = Shader::CreateFromSource("ColorShader", Application::GetAPI()->GetShaderFactory()->ColorShader(), bufferprop);

		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), bufferprop);
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(ulong));


		m_ArrayBuffer = ArrayBuffer::Create(vertexBuffer, indexBuffer);

	//	shader->Bind();
		//shader->SetUniform3f("u_Color", glm::vec3(1.0f, 0.1f, 0.3f));


		//m_Buffer->AddVertexBuffer(m_VertexBuffer);
		//m_Buffer->SetIndexBuffer(m_IndexBuffer);

		/*Texture* texture = Texture::Create("res/Texture/crow.png", TextureProperties(CROW_NEAREST_MIPMAP_NEAREST, CROW_NEAREST, CROW_CLAMP_TO_EDGE, CROW_CLAMP_TO_EDGE));
		Texture* texture2 = Texture::Create("res/Texture/crow2.png", TextureProperties(CROW_NEAREST_MIPMAP_NEAREST, CROW_NEAREST, CROW_CLAMP_TO_EDGE, CROW_CLAMP_TO_EDGE));
		Texture* playerTexture = Texture::Create("res/Texture/Player.png", TextureProperties(CROW_NEAREST_MIPMAP_NEAREST, CROW_NEAREST, CROW_CLAMP_TO_EDGE, CROW_CLAMP_TO_EDGE));

		Shader* shader = Shader::CreateFromPath("Basic2", "res/Shader/Basic.glsl");

		Shader* playerShader = Shader::CreateFromPath("Player", "res/Shader/Basic.glsl");

		glm::mat4 translation = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		translation *= glm::translate(translation, glm::vec3(1.0f, 0.0f, 0.0f));

		ShaderManager::PushShader(shader);
		ShaderManager::PushShader(playerShader);

		m_Objects.push_back(new Object2D(m_Buffer, shader, { texture }));
		m_Objects[0]->SetPosition(glm::vec3(0, 0, 0));

		m_Objects[0]->SetCollision(glm::vec2(1.0f, 1.0f));
		
		for (int x = 0; x < 80; x++)
		{
			for (int y = 0; y < 80; y++)
			{
				m_BatchingObjects.push_back(new Batchable2D(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(uint), shader, texture2, glm::vec3(x+1.0f, y, 0.0f)));
			}
		}

		m_Player = new Object2D(m_Buffer, playerShader, { playerTexture });
		m_Player->AddPosition(glm::vec3(2.0f, 2.0f, 0.0f));
		m_Player->SetCollision(glm::vec2(1.0f, 1.0f));
		m_Camera->SetCameraPos(glm::vec3(2.0f, 0.0f, 0.0f));
		m_Player->SetPosition(m_Camera->GetCameraPos());*/
	}

	Layer2D::~Layer2D()
	{
		//delete m_PSO;
		/*for(const auto object : m_Objects)
			delete object;
		for (const auto batch : m_BatchingObjects)
			delete batch;
		delete m_Player;*/
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

	void Layer2D::OnUpdate(float elapsed)
	{
		/*glm::vec3 pos = m_Camera->GetCameraPos();
		m_Camera->Update(elapsed);

		if (m_Player->IsColliding(m_Objects[0]))
			//m_Camera->SetCameraPos(pos);
		//m_Camera->SetCameraPos(glm::vec3(m_Player->GetCorrection(m_Objects[0]), 0.0f));
		m_Player->SetPosition(m_Camera->GetCameraPos());

		for (auto object : m_Objects)
		{
			ShaderManager::GetShader("Basic2")->Bind();
			ShaderManager::GetShader("Basic2")->SetUniformMat4("u_MVP", m_Camera->GetProjectionViewMatrix() * object->GetModelMatrix());
			ShaderManager::GetShader("Player")->Bind();
			ShaderManager::GetShader("Player")->SetUniformMat4("u_MVP", m_Camera->GetProjectionMatrix() * object->GetModelMatrix());
		}

		for (auto object : m_BatchingObjects)
		{
			object->m_Shader->Bind();
			object->m_Shader->SetUniformMat4("u_MVP", m_Camera->GetProjectionViewMatrix());
		}*/
	}

	void Layer2D::OnRender()
	{
		m_Shader->Bind();
		m_ArrayBuffer->Bind();

		Application::GetAPI()->DrawIndices(m_ArrayBuffer->GetCount());

		/*m_Renderer->Begin();
		for (auto batching : m_BatchingObjects)
		{
			m_Renderer->BatchSubmit(batching);
		}
		m_Renderer->Submit(m_Objects);
		m_Renderer->Submit(m_Player);
		m_Renderer->End();
		m_Renderer->Flush();*/
	}

class Game : public Crow::Application {
	
public:
	Game()
		: Application("The Crows 2D", Crow::Platform::GraphicAPI::DIRECTX, Platform::ApplicationAPI::WINDOWS)
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