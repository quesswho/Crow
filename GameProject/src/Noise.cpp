#include "Noise.h"

using namespace Crow;
using namespace Math;


Noise::Noise()
{
	Application::GetAPI()->ClearColor(0.5f, 0.7f, 0.5f);
	Application::GetAPI()->EnableDepthTest();
	Application::GetAPI()->EnableBlending();

	m_PostFXShader = Shader::CreateFromPath("PostFX", "res/Shader/Postfx/Noise.x", { { "POSITION", 2 }, { "TEXCOORD", 2 } });
	m_PostFX = PostEffect::CreateFromShader(m_PostFXShader);
	Application::BindPostEffect(m_PostFX);
	m_PostFXShader->Bind();
	m_PostFXShader->SetUniformValue("u_Resolution", Vec2(SCREENX, SCREENY));
}

Noise::~Noise()
{}

void Noise::OnEvent(Event& e)
{
	auto key = e.GetKey().x;
	auto action = e.GetKey().y;
	switch (e.m_Type)
	{
		case KEY:
			CR_GAME_INFO("Keyboard: {}", key);
			if (key == CROW_KEY_ESCAPE && action == CROW_KEY_PRESS)
				Crow::Application::Shutdown();
	}
}

void Noise::OnUpdate(float elapsed)
{
	m_PostFXShader->Bind();
	m_PostFXShader->SetUniformValue("u_Mousepos", Vec2(Input::GetMousePosition().x, Input::GetMousePosition().y));
}

void Noise::OnRender()
{
	m_PostFXShader->Bind();
}