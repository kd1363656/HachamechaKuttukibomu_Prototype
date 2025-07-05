#include "JsonUtility.h"

nlohmann::json JsonUtility::LoadJsonFile(const std::string& FilePath)
{
	std::filesystem::path path_(FilePath);
	if (path_.extension() != ".json")
	{
		path_ += ".json";
	}

	std::ifstream ifs_(path_);

	if (ifs_.fail()) { assert(false && "Jsonファイルが読み込めませんでした、ファイルパスを確認してください"); }

	nlohmann::json json_;
	ifs_ >> json_;
	ifs_.close();

	return json_;
}

void JsonUtility::SaveJsonFile(const nlohmann::json& Json, const std::string& FilePath)
{
	std::filesystem::path path_(FilePath);

	if (path_.extension() != ".json")
	{
		path_ += ".json";
	}

	std::ofstream ofs_(path_, std::ios::out);

	ofs_ << Json.dump(4);
	ofs_.close();
}