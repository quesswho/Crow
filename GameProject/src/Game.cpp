#include "Game.h"

#include "glm/vec3.hpp"
#include "Crow/Graphics/ShaderFactory.h"

using namespace Crow;



	Layer2D::Layer2D()
		: m_Camera(new OrthographicCamera(glm::vec2(0.0f, 0.0f), 2.0f, 2.0f, 1.0f))
	{
		Application::GetAPI()->ClearColor(0.5, 0.7, 0.5);

		m_Renderer = std::make_unique<Renderer2D>();

		BufferProperties bufferprop = { 
			{ 3 }, //vertices
			{ 2 }  //uvs
		};

		ArrayBuffer* m_Buffer = ArrayBuffer::Create();

		const float vertices[4*5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,
			0.5f,   0.5f, 0.0f, 1.0f, 1.0f
		};

		const uint indices[] = {
			0,1,2, 3,2,1
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), bufferprop);
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));


		m_Buffer->AddVertexBuffer(vertexBuffer);
		m_Buffer->SetIndexBuffer(indexBuffer);

		Texture* texture = Texture::Create("res/Texture/crow.png", TextureProperties(CROW_NEAREST_MIPMAP_NEAREST, CROW_NEAREST, CROW_CLAMP_TO_EDGE, CROW_CLAMP_TO_EDGE));
		Texture* texture2 = Texture::Create("res/Texture/crow2.png", TextureProperties(CROW_NEAREST_MIPMAP_NEAREST, CROW_NEAREST, CROW_CLAMP_TO_EDGE, CROW_CLAMP_TO_EDGE));

		Shader* shader2 = Shader::CreateFromPath("Basic2", "res/Shader/Basic.glsl");

		glm::mat4 translation = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		translation *= glm::translate(translation, glm::vec3(1.0f, 0.0f, 0.0f));

		ShaderManager::PushShader(shader2);

		m_Objects.push_back(new Object2D(m_Buffer, shader2, { texture2 }));
		m_Objects[0]->SetPosition(glm::vec3(1, 0, 0));
		m_Objects[0]->SetScale(glm::vec3(1.0f, 2.0f, 1.0f));

		m_Objects.push_back(new Object2D(m_Buffer));
		m_Objects[1]->AddTexture(texture);
		m_Objects[1]->AddTexture(texture2);
		
	}

	Layer2D::~Layer2D()
	{
		for(const auto object : m_Objects)
			delete object;
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

	void Layer2D::OnUpdate(double elapsed)
	{
		m_Camera->Update(elapsed);
		for (auto object : m_Objects)
		{
			object->m_Shader->Bind();
			object->m_Shader->SetUniformMat4("u_MVP", m_Camera->GetProjectionViewMatrix() * object->GetModelMatrix());
		}
		//m_Objects[1]->SetRotation(m_Objects[1]->GetRotation() + 0.5f * elapsed);
	}

	void Layer2D::OnRender()
	{
		m_Renderer->Submit(m_Objects);
		m_Renderer->Flush();
	}

class Game : public Crow::Application {
	
public:
	Game()
		: Application("The Crows 2D", Crow::Platform::RenderAPI::OPENGL)
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