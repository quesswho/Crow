#pragma once
#include "Crow/Common.h"

#include "Renderer/Buffer.h"

#include <glm/mat2x2.hpp>
#include "Crow/Math/Maths.h"

namespace Crow {



	struct Shader {

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

		virtual ~Shader() = default;

		static Shader* CreateFromPath(const char* name, const char* path, const BufferProperties& shaderInput);
		static Shader* CreateFromSource(const char* name, std::string& source, const BufferProperties& shaderInput);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


		virtual void ReloadFromPath(const char* path) = 0;
		virtual void ReloadFromSource(std::string& source) = 0;

		virtual void CreateConstantBuffers() = 0;

		virtual const char* GetName() const = 0;

		virtual void SetUniformValue(const char* location, const int value) = 0;
		virtual void SetUniformValue(const char* location, const float value) = 0;
		virtual void SetUniformValue(const char* location, const Math::Vec2<float>&  value) = 0;
		virtual void SetUniformValue(const char* location, const Math::Vec3<float>& value) = 0;
		virtual void SetUniformValue(const char* location, const Math::Vec4<float>& value) = 0;
		virtual void SetUniformValue(const char* location, const glm::mat2x2& value) = 0;
		virtual void SetUniformValue(const char* location, const Math::Mat3<float>& value) = 0;
		virtual void SetUniformValue(const char* location, const Math::Mat4<float>& value) = 0;

		virtual void SetUniformStruct(const char* location, void* data) = 0;
	};
}