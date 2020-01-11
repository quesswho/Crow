#include "Game.h"

#include "Crow/Graphics/ShaderFactory.h"

using namespace Crow;
using namespace Math;
	Layer2D::Layer2D()
		: m_Camera(new FirstPersonCamera(Vec3(0.0f, 1.0f, -3.0f), 1080.0f / 720.0f, 0.05f, 3.0f))
	{

		m_Model *= Mat4::Scale(Vec3(1.0f, 1.0f, 1.0f));

		Application::GetAPI()->ClearColor(0.5f, 0.7f, 0.5f);
		Application::GetAPI()->EnableDepthTest();

		BufferProperties bufferprop = { 
			{ "POSITION", 3 }, //vertices
			{ "TEXCOORD", 2 }  //uvs
		};

		float vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 1.0f
		};

		ulong indices[] {
			0,1,2,
			0,2,3,
			
			7,6,4,
			5,4,6,

			1,0,4,
			1,4,5,

			2,6,3,
			3,6,7,

			2,1,5,
			2,5,6,

			3,7,0,
			0,7,4
		};



		m_Shader = Shader::CreateFromSource("BasicLightShader", Application::GetAPI()->GetShaderFactory()->TextureShader(), bufferprop);

		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), bufferprop);
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(ulong));

		m_Texture = Texture::Create("res/Texture/crow2.png", TextureProperties());


		m_ArrayBuffer = ArrayBuffer::Create(vertexBuffer, indexBuffer);
		
		m_Light = new Light(glm::vec2(0.0f, 0.0f), glm::vec4(0.5f, 0.1f, 0.5f, 1.0f));
	}

	Layer2D::~Layer2D()
	{}

	void Layer2D::OnEvent(Event& e)
	{
		auto key = e.GetKey().x;
		auto action = e.GetKey().y;
		switch (e.m_Type)
		{
		case MOUSEPOS: // key = x, action = y
			m_Camera->UpdateRotation();
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

		if (Input::IsKeyPressed(CROW_KEY_UP))
			m_Light->m_Pos.y += elapsed * 1.0f;
		if (Input::IsKeyPressed(CROW_KEY_DOWN))
			m_Light->m_Pos.y-= elapsed * 1.0f;

		if (Input::IsKeyPressed(CROW_KEY_LEFT))
			m_Light->m_Pos.x -= elapsed * 1.0f;
		if (Input::IsKeyPressed(CROW_KEY_RIGHT))
			m_Light->m_Pos.x += elapsed * 1.0f;

		if (Input::IsKeyPressed(CROW_KEY_Q))
			m_Model *= Mat4::Rotate(90.0f * elapsed, Vec3(1.0f, 1.0f, 0.0f));
		if (Input::IsKeyPressed(CROW_KEY_E))
			m_Model *= Mat4::Rotate(-90.0f * elapsed, Vec3(1.0f, 1.0f, 0.0f));
		if (Input::IsKeyPressed(CROW_KEY_R))
			m_Camera->SetViewDir(Vec3(0.0f, 0.0f, 1.0f));


		m_Camera->Update(elapsed);
	}

	void Layer2D::OnRender()
	{
		m_Shader->Bind();
		m_ArrayBuffer->Bind();

		m_Shader->SetUniformValue("u_VP",  m_Camera->GetCameraMatrix());
		//m_Shader->SetUniformStruct("u_Light", m_Light);
		//m_Shader->SetUniformValue("u_Texture", 0);
		m_Shader->SetUniformValue("u_Model", m_Model * Mat4::Translate(Vec3(1.0f, 3.0f, 5.0f)));
		
		m_Texture->Bind();
		Application::GetAPI()->DrawIndices(m_ArrayBuffer->GetCount());

		//Application::GetAPI()->DrawIndices(m_ArrayBuffer->GetCount());
	}

class Game : public Crow::Application {
	
public:
	Game()
		: Application(WindowProperties("The Crows 2D", 1080, 720), Crow::Platform::GraphicAPI::DIRECTX11, Platform::ApplicationAPI::WINDOWS)
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