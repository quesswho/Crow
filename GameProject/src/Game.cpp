#include "Game.h"

using namespace Crow;
using namespace Math;

	Layer2D::Layer2D()
		: m_Camera(new FirstPersonCamera(Vec3(0.0f, 1.0f, -3.0f), 1080.0f / 720.0f, 0.02f, 3.0f))
	{
		Application::GetAPI()->ClearColor(0.5f, 0.7f, 0.5f);
		Application::GetAPI()->EnableDepthTest();
		Application::GetAPI()->EnableBlending();

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

		m_OpenSansFont = Font::Create("res/Font/OpenSans.ttf");

		m_ArrayBuffer = ArrayBuffer::Create(vertexBuffer, indexBuffer);
		
		m_PostFXShader = Shader::CreateFromPath("PostFX", "res/Shader/Postfx/GammaTest.x", { { "POSITION", 2 }, { "TEXCOORD", 2 } });
		m_PostFX = PostEffect::CreateFromShader(m_PostFXShader);

		m_Light = new Light(glm::vec2(0.0f, 0.0f), glm::vec4(0.5f, 0.1f, 0.5f, 1.0f));

		Application::BindPostEffect(m_PostFX);

		m_Gamma = 2.2f;
		m_PostFXShader->Bind();
		m_PostFXShader->SetUniformValue("u_Gamma", m_Gamma);
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
			m_Gamma += 1 * elapsed;
		if (Input::IsKeyPressed(CROW_KEY_DOWN))
			m_Gamma -= 1 * elapsed;
		if (Input::IsKeyPressed(CROW_KEY_R))
			m_Camera->SetRotation(Vec2(90.0f, 0.0f));

		m_PostFXShader->Bind();
		m_PostFXShader->SetUniformValue("u_Gamma", m_Gamma);

		m_Camera->Update(elapsed);
	}

	void Layer2D::OnRender()
	{
		m_Shader->Bind();
		m_Shader->SetUniformValue("u_VP",  m_Camera->GetCameraMatrix());
		m_Shader->SetUniformValue("u_Model", m_Model * Mat4::Translate(Vec3(1.0f, 3.0f, 5.0f)));
		
		m_Texture->Bind();
		m_ArrayBuffer->Bind();
		Application::GetAPI()->DrawIndices(m_ArrayBuffer->GetCount());

		m_PostFXShader->Bind();

		m_OpenSansFont->DrawDynamic(std::string("FPS: ").append(std::to_string(Application::m_FramesPerSecond)).c_str(), Vec2(20.0f, 20.0f), 1);
	}

class Game : public Crow::Application {
	
public:
	Game()
		: Application(WindowProperties("The Crows 2D", 1080, 720), Platform::ApplicationAPI::WINDOWS)
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