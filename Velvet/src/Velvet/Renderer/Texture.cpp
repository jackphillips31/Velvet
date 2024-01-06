#include <vlpch.h>
#include "Velvet/Renderer/Texture.h"

#include "Velvet/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGlTexture.h"

namespace Velvet {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	VL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path);
		}

		VL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& name, Ref<RawImageData>& imageData)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	VL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(name, imageData);
		}

		VL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& name, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	VL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(name, width, height);
		}

		VL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void Texture2DLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
	{
		VL_PROFILE_FUNCTION();

		VL_CORE_ASSERT(!Exists(name), "Texture with this name already exists!");
		VL_CORE_INFO("Adding {} to Texture2DLibrary!", name);
		m_Textures2D[name] = texture;
	}

	void Texture2DLibrary::Add(const Ref<Texture2D>& texture)
	{
		Add(texture->GetName(), texture);
	}

	Ref<Texture2D> Texture2DLibrary::Load(const std::string& filepath)
	{
		Ref<Texture2D> result = Texture2D::Create(filepath);
		Add(result);
		return result;
	}

	Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, const Ref<RawImageData>& imageData)
	{
		Ref<Texture2D> result = Texture2D::Create((std::string&)name, (Ref<RawImageData>&)imageData);
		Add(result);
		return result;
	}

	Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, uint32_t width, uint32_t height)
	{
		Ref<Texture2D> result = Texture2D::Create(name, width, height);
		Add(result);
		return result;
	}

	Ref<Texture2D> Texture2DLibrary::LoadSimple(const std::string& name, uint32_t data)
	{
		Ref<Texture2D> result = Texture2D::Create(name, 1, 1);
		result->SetData(&data, sizeof(uint32_t));
		Add(result);
		return result;
	}

	Ref<Texture2D> Texture2DLibrary::Get(const std::string& name)
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_ASSERT(Exists(name), "Texture not found!");
		return m_Textures2D[name];
	}

	bool Texture2DLibrary::Exists(const std::string& name) const
	{
		return m_Textures2D.find(name) != m_Textures2D.end();
	}

}