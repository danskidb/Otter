#include "Filesystem.h"
#include "Otter/Utils/Log.h"
#include <fstream>

namespace Otter {

	const fs::path Filesystem::GetAssetDirectoryEngine()
	{
		fs::path pth = fs::current_path();
		pth /= assetDirectory;
		pth /= engineSubDirectory;
		pth.make_preferred();

		OT_ASSERT(fs::exists(pth), "Path " + pth.string() + " does not exist!");

		return fs::canonical(pth);
	}

	const fs::path Filesystem::GetAssetDirectoryGame()
	{
		fs::path pth = fs::current_path();
		pth /= assetDirectory;
		pth /= gameSubDirectory;
		pth.make_preferred();

		OT_ASSERT(fs::exists(pth), "Path " + pth.string() + " does not exist!");

		return fs::canonical(pth);
	}

	std::string Filesystem::ReadFile(fs::path &filePath)
	{
		fs::path fp = filePath.make_preferred();
		OT_ASSERT(fs::exists(fp), "Path " + fp.string() + " does not exist!");
		OT_CORE_INFO("Attempting to read " + fp.string());

		// Open the stream to 'lock' the file.
		std::ifstream fileStream(fs::canonical(filePath), std::ios::in | std::ios::binary);

		// Obtain the size of the file.
		const auto fileSize = fs::file_size(filePath);

		// Create a buffer.
		std::string result(fileSize, '\0');

		// Read the whole file into the buffer.
		fileStream.read(result.data(), fileSize);
		fileStream.close();

		return result;
	}

}