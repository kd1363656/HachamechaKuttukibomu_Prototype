#include "FileSystem.h"

nlohmann::json FileSystem::LoadJsonFile(const std::string& FilePath)
{
	std::filesystem::path path_(FilePath);
	if (path_.extension() != JSON_EXTENSION)
	{
		path_ += JSON_EXTENSION;
	}

	std::ifstream ifs_(path_);

	if (ifs_.fail()) { assert(false && "Jsonファイルが読み込めませんでした、ファイルパスを確認してください"); }

	nlohmann::json json_;
	ifs_ >> json_;
	ifs_.close();

	return json_;
}

void FileSystem::SaveJsonFile(const nlohmann::json& Json, const std::string& FilePath)
{
	std::filesystem::path path_(FilePath);

	if (path_.extension() != JSON_EXTENSION)
	{
		path_ += JSON_EXTENSION;
	}

	std::ofstream ofs_(path_, std::ios::out);

	ofs_ << Json.dump(4);
	ofs_.close();
}

std::vector<std::filesystem::path> FileSystem::GetAllFilePathsInDirectory(const std::string& DirectoryPath)
{
	std::vector<std::filesystem::path> filePath_;

	// "DirectoryPath"が存在するか、またそれはディレクトリであるかを確認
	if(!std::filesystem::exists(DirectoryPath) || !std::filesystem::is_directory(DirectoryPath))
	{
		KdDebugGUI::Instance().AddLog("指定されたパスは有効なディレクトリではありません");
		return filePath_;
	}

	// もしディレクトリであることが確認できればディレクトリ以下のファイルパスを保存
	for(const auto& entry_ : std::filesystem::recursive_directory_iterator(DirectoryPath))
	{
		if(std::filesystem::is_regular_file(entry_.path()))
		{
			filePath_.emplace_back(entry_.path());
		}
	}

	return filePath_;
}