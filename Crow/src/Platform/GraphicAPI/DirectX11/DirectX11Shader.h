#pragma once

#include "dx11.h"

#include "Crow/Graphics/Renderer/BufferProp.h"

#include "Crow/Math/Maths.h"

namespace Crow {
	namespace Platform {

		class DirectX11Shader {
		public:
			enum class ShaderType { UNKNOWN = -1, VERTEX = 0, FRAGMENT = 1 };

			enum class UniformType
			{
				UNKNOWN = -1,
				INT,
				FLOAT,
				FLOAT2,
				FLOAT3,
				FLOAT4,
				MAT2,
				MAT3,
				MAT4
			};
		private:
			struct DX11ConstantBuffer {
				DX11ConstantBuffer(int size, int reg, ShaderType type)
					: m_Size(size), m_Reg(reg), m_Type(type), m_ConstantBuffer()
				{}

				~DX11ConstantBuffer()
				{
					m_ConstantBuffer->Release();
				}

				int m_Size;
				int m_Reg;

				ShaderType m_Type;

				ID3D11Buffer* m_ConstantBuffer;
			};
		private:
			ID3D11VertexShader* m_VertexShader;
			ID3D11PixelShader* m_FragmentShader;
			ID3D10Blob* m_CompiledVertexShader;
			ID3D10Blob* m_CompiledFragmentShader;
			ID3D11InputLayout* m_VertexInputLayout;

			std::vector<DX11ConstantBuffer*> m_ConstanBuffers;
			std::unordered_map<std::string, int> m_ConstantBufferLocations;

			const char* m_Name;
		public:
			explicit DirectX11Shader(const char* name, const char* path, const BufferProperties& shaderInput); // File path
			explicit DirectX11Shader(const char* name, std::string& source, const BufferProperties& shaderInput); // Shader code

			~DirectX11Shader();

			static DirectX11Shader* CreateFromPath(const char* name, const char* path, const BufferProperties& shaderInput) { return new DirectX11Shader(name, path, shaderInput); }
			static DirectX11Shader* CreateFromSource(const char* name, std::string& source, const BufferProperties& shaderInput) { return new DirectX11Shader(name, source, shaderInput); }

			void Bind() const;
			void Unbind() const;

			void ReloadFromPath(const char* path);
			void ReloadFromSource(std::string& source);

			void CreateConstantBuffers()  {}

			const char* GetName() const { return m_Name; }

			void SetUniformValue(const char* location, const int value);
			void SetUniformValue(const char* location, const float value);
			void SetUniformValue(const char* location, const Math::TVec2<float>& value);
			void SetUniformValue(const char* location, const Math::TVec3<float>& value);
			void SetUniformValue(const char* location, const Math::TVec4<float>& value);
			void SetUniformValue(const char* location, const Math::Mat2& value);
			void SetUniformValue(const char* location, const Math::Mat3& value);
			void SetUniformValue(const char* location, const Math::Mat4& value);

			void SetUniformStruct(const char* location, void* data);

		private:
			DXGI_FORMAT ConvertToDXGIFormat(int componentCount);

			void Init(std::string& fileSource);
			void FindConstantBuffer(std::string& vertex, std::string& fragment);
			bool CompileShader(const char* vertex, const char* fragment);
			void InitPipeline();

			int GetLocation(const char* location);
		private:
			static UniformType StringToUniformType(std::string& string);
			static uint UniformTypeToSize(UniformType type);
		private:
			const BufferProperties m_ShaderInput;
		};
	}
}