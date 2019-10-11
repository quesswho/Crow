#pragma once

#include "Crow/Graphics/Shader.h"

#include "DirectXRenderAPI.h"

namespace Crow {
	namespace Platform {

		class DirectXShader : public Shader {
		public:
			enum UniformType
			{
				UNKNOWN = -1,
				INT,
				FLOAT,
				FLOAT2,
				FLOAT3,
				FLOAT4,
				MAT3,
				MAT4
			};

		private:

			struct IDXSingleFieldUniform {};

			template<typename T>
			struct DXSingleFieldUniform : public IDXSingleFieldUniform {
				DXSingleFieldUniform(T data)
					: m_Data(data)
				{}

				T m_Data;
			};

			struct IDXSingleFieldCBuffer {

				~IDXSingleFieldCBuffer() 
				{
					m_ConstantBufferUploadHeap->Release();
				}

				virtual int GetReg() const = 0;
				virtual int GetSize() const = 0;

				ID3D12Resource* m_ConstantBufferUploadHeap;
				D3D12_GPU_DESCRIPTOR_HANDLE m_ConstantBufferHandle;
				UINT* m_GPUAddress;
			};

			template<typename T>
			struct DXSingleFieldCBuffer : public IDXSingleFieldCBuffer {
				DXSingleFieldCBuffer(T data, int reg)
					: m_Struct(new DXSingleFieldUniform<T>(data)), m_Reg(reg), m_Size(sizeof(T))
				{}

				virtual ~DXSingleFieldCBuffer()
				{
					delete m_Struct;
				}

				virtual int GetReg() const override { return m_Reg; }
				virtual int GetSize() const override { return m_Size; }

				IDXSingleFieldUniform* m_Struct;
			private:
				int m_Size;
				int m_Reg;
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

			std::vector<IDXSingleFieldCBuffer*> m_SingleUniformConstantBuffers;
			std::unordered_map<int, int> m_AllocateSingleUniformCBuffers; // reg, size
			std::unordered_map<std::string, int> m_AllocatedSingleUniformCbuffers;	// name, index
			
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
			
			virtual void CreateConstantBuffers(int frame) override;

			virtual const char* GetName() const { return m_Name; }

			virtual void SetUniformValue(const char* location, int value) override;
			virtual void SetUniformValue(const char* location, float value) override;
			virtual void SetUniformValue(const char* location, glm::vec2& value) override;
			virtual void SetUniformValue(const char* location, glm::vec3& value) override;
			virtual void SetUniformValue(const char* location, glm::vec4& value) override;
			virtual void SetUniformValue(const char* location, const glm::mat3x3& value) override;
			virtual void SetUniformValue(const char* location, const glm::mat4x4& value) override;

			virtual void SetUniformStruct(const char* location, const void* data) override {}

			D3D12_SHADER_BYTECODE GetVertexShader() { return m_CompiledVertexShader; }
			D3D12_SHADER_BYTECODE GetFragmentShader() { return m_CompiledFragmentShader; }
			int GetCBufferCount() const { return m_SingleUniformConstantBuffers.size(); }

		private:
			DXGI_FORMAT ConvertToDXGIFormat(int componentCount);

			void Init(std::string& fileSource);
			void InitPSO();
			void FindConstantBuffer(std::string& vertex);
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