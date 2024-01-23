#pragma once

#include "Velvet/Core/Core.h"

#include "Velvet/Core/ResourceManager.h"

namespace Velvet {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;

		virtual const std::string& GetName() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const std::string& name, Ref<RawImageData>& imageData);
		static Ref<Texture2D> Create(const std::string& name, uint32_t width, uint32_t height);
	};

	class Texture2DLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Texture2D>& texture);
		void Add(const Ref<Texture2D>& texture);
		Ref<Texture2D> Load(const std::string& filepath);
		Ref<Texture2D> Load(const std::string& name, const Ref<RawImageData>& imageData);
		Ref<Texture2D> Load(const std::string& name, uint32_t width, uint32_t height);
		Ref<Texture2D> LoadSimple(const std::string& name, uint32_t data);
		Ref<Texture2D> Get(const std::string& name);

		bool Exists(const std::string& name) const;
		void Clear();
	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures2D;
	};

}