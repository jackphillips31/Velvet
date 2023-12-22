#pragma once

namespace Velvet {
	
	struct RawImageData
	{
		RawImageData(const unsigned char* data, const int size);

		const unsigned char* Data;
		int Size;
	};

	class ResourceManager
	{
	public:
		static void AddImage(const std::string& name, const Ref<RawImageData>& newImage);
		static Ref<RawImageData> LoadImage(const std::string& name, const unsigned char* rawData, const int size);
		static Ref<RawImageData> GetImage(const std::string& name);
		static bool Exists(const std::string& name);

		static void Shutdown();
	private:
		static std::unordered_map<std::string, Ref<RawImageData>> m_Images;
	};

}