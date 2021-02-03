#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace Otter {

	class Filesystem
	{
	public:
		inline static std::string assetDirectory = "Assets/";	//todo: handling universal of execution location
		inline static std::string engineSubDirectory = "Otter/";
		inline static std::string gameSubDirectory = "Game/";

		// Returns the path to the Otter asset directory
		static fs::path GetAssetDirectoryEngine();

		// Returns the path to the Game asset directory
		static fs::path GetAssetDirectoryGame();

		// Reads an entire file into a string.
		static std::string ReadFile(fs::path filePath);
	};
}