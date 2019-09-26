#pragma once

#include "Crow/Graphics/Shader.h"

#include "DirectXRenderAPI.h"

namespace Crow {
	namespace Platform {

		class DirectXShader : public Shader {
		private:
			ID3DBlob* m_VertexShader;
			ID3DBlob* m_FragmentShader;
			D3D12_SHADER_BYTECODE m_CompiledVertexShader;
			D3D12_SHADER_BYTECODE m_CompiledFragmentShader;

			ID3D12PipelineState* m_PSO;
			ID3D12RootSignature* m_RootSignature;

			const char* m_Name;
			std::unordered_map<const char*, int> m_UniformLocations;
		public:
			explicit DirectXShader(const char* name, const char* path, const BufferProperties& shaderInput); // File path
			explicit DirectXShader(const char* name, std::string& source, const BufferProperties& shaderInput); // Shader code

			~DirectXShader() override;

			static Shader* CreateDirectXShaderFromPath(const char* name, const char* path, const BufferProperties& shaderInput) { return new DirectXShader(name, path, shaderInput); }
			static Shader* CreateDirectXShaderFromSource(const char* name, std::string& source, const BufferProperties& shaderInput) { return new DirectXShader(name, source, shaderInput); }

			virtual void Bind() const;
			virtual void Unbind() const;

			virtual void ReloadFromPath(const char* path) override;
			virtual void ReloadFromSource(std::string& source) override;

			virtual const char* GetName() const { return m_Name; }

			virtual void SetUniform1i(const char* location, int value);
			virtual void SetUniform1f(const char* location, float value);
			virtual void SetUniform2f(const char* location, glm::vec2& value);
			virtual void SetUniform3f(const char* location, glm::vec3& value);
			virtual void SetUniform4f(const char* location, glm::vec4& value);
			virtual void SetUniformMat4(const char* location, const glm::mat4x4& value);

			D3D12_SHADER_BYTECODE GetVertexShader() { return m_CompiledVertexShader; }
			D3D12_SHADER_BYTECODE GetFragmentShader() { return m_CompiledFragmentShader; }
		private:
			DXGI_FORMAT ConvertToDXGIFormat(int componentCount);

			void Init(std::string& fileSource);
			void InitPSO();
			void CompileShader(const char* vertex, const char* fragment);

			int GetLocation(const char* location);

		private:
			const BufferProperties m_ShaderInput;
		};
	}
}