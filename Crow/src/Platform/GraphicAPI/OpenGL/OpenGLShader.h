#pragma once
#include "Crow/Common.h"

#include "Crow/Graphics/Renderer/BufferProp.h"
#include "Crow/Math/Maths.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace Crow {
	namespace Platform {

		class OpenGLShader {
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

			struct TempUniform {
				TempUniform(const std::string& name, UniformType type)
					: m_Name(name), m_Type(type)
				{}
				
				const std::string m_Name;
				UniformType m_Type;
			};

			struct TempShaderUniformStruct {

				TempShaderUniformStruct(const std::string& name, std::vector<TempUniform> uniforms, int size)
					: m_Name(name), m_Uniforms(uniforms), m_Size(size)
				{}

				const std::string m_Name;
				std::vector<TempUniform> m_Uniforms;
				int m_Size;
			};

			struct Uniform {
				Uniform(int location, UniformType type)
					: m_Location(location), m_Type(type)
				{}

				int m_Location;
				UniformType m_Type;
			};

			struct ShaderUniformStruct {

				ShaderUniformStruct(std::string& name, std::vector<Uniform> uniforms, int size)
					: m_Name(name), m_Uniforms(uniforms), m_Size(size)
				{}

				std::string m_Name;
				std::vector<Uniform> m_Uniforms;
				int m_Size;
			};

		private:
			uint m_ShaderID;
			const char* m_Name;
			std::unordered_map<const char*, int> m_UniformLocations;
			std::vector<ShaderUniformStruct> m_UniformStructs;
			std::unordered_map<std::string, int> m_UniformStructLocations;
		public:
			explicit OpenGLShader(const char* name, const char* path, const BufferProperties& shaderInput); // File path
			explicit OpenGLShader(const char* name, std::string& source, const BufferProperties& shaderInput); // Shader code

			~OpenGLShader();

			static OpenGLShader* CreateFromPath(const char* name, const char* path, const BufferProperties& shaderInput) { return new OpenGLShader(name, path, shaderInput); }
			static OpenGLShader* CreateFromSource(const char* name, std::string& source, const BufferProperties& shaderInput) { return new OpenGLShader(name, source, shaderInput); }

			void Bind() const;
			void Unbind() const;

			void ReloadFromPath(const char* path);
			void ReloadFromSource(std::string& source);

			void CreateConstantBuffers() {}

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

			inline uint GetHandle() const { return m_ShaderID; }
		private:
			static int StringToUniformTypeSize(std::string_view string);
			static UniformType StringToUniformType(std::string_view string);
			static int UniformTypeToUniformTypeSize(UniformType type);

			static inline float BytesToFloat(BYTE* bytes)
			{
				float result;
				memcpy(&result, bytes, 4);
				return result;
			}

			void Init(std::string& fileSource);
			void CompileShader(const char* vertex, const char* fragment);
			void FindUniformStructs(std::string& source);

			int GetLocation(const char* location);
		};
	}
}