#pragma once

#include "dx12.h"

#include "Crow/Graphics/Renderer/BufferProp.h"
#include "Crow/Math/Maths.h"

#include <unordered_map>

namespace Crow {
	namespace Platform {

		class DirectX12Shader {
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
			struct DX12ConstantBuffer {

				DX12ConstantBuffer(int size, int reg)
					: m_Size(size), m_Reg(reg), m_GPUAddress(0), m_ConstantBufferHandle(), m_ConstantBufferUploadHeap(0)
				{}

				~DX12ConstantBuffer()
				{
					m_ConstantBufferUploadHeap->Release();
					delete m_GPUAddress;
				}

				int m_Size;
				int m_Reg;

				ID3D12Resource* m_ConstantBufferUploadHeap;
				D3D12_GPU_DESCRIPTOR_HANDLE m_ConstantBufferHandle;
				UINT* m_GPUAddress;
			};

		private:
			ID3DBlob* m_VertexShader;
			ID3DBlob* m_FragmentShader;
			D3D12_SHADER_BYTECODE m_CompiledVertexShader;
			D3D12_SHADER_BYTECODE m_CompiledFragmentShader;

			ID3D12PipelineState* m_PSO;
			ID3D12RootSignature* m_RootSignature;

			const char* m_Name;
			int m_ConstantBufferShaderType; // Vertex 1, Fragment 2, Both 3

			std::vector<DX12ConstantBuffer*> m_UniformConstantBuffers;
			std::unordered_map<std::string, int> m_ConstantBufferLocations;	// name, index
			
		public:
			explicit DirectX12Shader(const char* name, const char* path, const BufferProperties& shaderInput); // File path
			explicit DirectX12Shader(const char* name, std::string& source, const BufferProperties& shaderInput); // Shader code

			~DirectX12Shader();

			static DirectX12Shader* CreateFromPath(const char* name, const char* path, const BufferProperties& shaderInput) { return new DirectX12Shader(name, path, shaderInput); }
			static DirectX12Shader* CreateFromSource(const char* name, std::string& source, const BufferProperties& shaderInput) { return new DirectX12Shader(name, source, shaderInput); }

			void Bind() const;
			void Unbind() const;

			void ReloadFromPath(const char* path);
			void ReloadFromSource(std::string& source);
			
			void CreateConstantBuffers();

			const char* GetName() const { return m_Name; }

			void SetUniformValue(const char* location, const int value);
			void SetUniformValue(const char* location, const float value);
			void SetUniformValue(const char* location, const Math::TVec2<float>&  value);
			void SetUniformValue(const char* location, const Math::TVec3<float>& value);
			void SetUniformValue(const char* location, const Math::TVec4<float>& value);
			void SetUniformValue(const char* location, const Math::Mat2& value);
			void SetUniformValue(const char* location, const Math::Mat3& value);
			void SetUniformValue(const char* location, const Math::Mat4& value);

			void SetUniformStruct(const char* location, void* data);

			D3D12_SHADER_BYTECODE GetVertexShader() { return m_CompiledVertexShader; }
			D3D12_SHADER_BYTECODE GetFragmentShader() { return m_CompiledFragmentShader; }
			size_t GetCBufferCount() const { return m_UniformConstantBuffers.size(); }

		private:
			DXGI_FORMAT ConvertToDXGIFormat(int componentCount);

			void Init(std::string& fileSource);
			void InitPSO();
			void FindConstantBuffer(std::string& vertex, std::string& fragment);
			void CompileShader(const char* vertex, const char* fragment);

			int GetLocation(const char* location);

		private:
			static UniformType StringToUniformType(std::string& string);
			static uint UniformTypeToSize(UniformType type);

		private:
			const BufferProperties m_ShaderInput;
		};
	}
}