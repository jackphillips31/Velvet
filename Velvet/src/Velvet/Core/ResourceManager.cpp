#include <vlpch.h>
#include "Velvet/Core/ResourceManager.h"

namespace Velvet {

	RawImageData::RawImageData(const unsigned char* data, int size)
		: Data(data), Size(size)
	{
	}

	void ResourceManager::AddImage(const std::string& name, const Ref<RawImageData>& newImage)
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_ASSERT(!Exists(name), "Image already exists!");
		VL_CORE_WARN("Adding {} to ResourceManager", name);
		m_Images[name] = newImage;
	}

	Ref<RawImageData> ResourceManager::LoadImage(const std::string& name, const unsigned char* rawData, const int size)
	{
		Ref<RawImageData> newImage = CreateRef<RawImageData>(rawData, size);
		AddImage(name, newImage);
		return newImage;
	}

	Ref<RawImageData> ResourceManager::GetImage(const std::string& name)
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_ASSERT(Exists(name), "Image not found!");
		return m_Images[name];
	}

	bool ResourceManager::Exists(const std::string& name)
	{
		return m_Images.find(name) != m_Images.end();
	}

	void ResourceManager::DeleteImage(const std::string& name)
	{
		m_Images.erase(name);
	}

	void ResourceManager::Shutdown()
	{
		m_Images.clear();
	}

	std::unordered_map<std::string, Velvet::Ref<Velvet::RawImageData>> ResourceManager::m_Images;

}