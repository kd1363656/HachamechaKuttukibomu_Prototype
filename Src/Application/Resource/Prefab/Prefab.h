#pragma once

class Prefab
{
public:

	Prefab () = default;
	~Prefab() = default;

	void Init(std::string TypeName);

private:

	const std::string COMMON_FILE_PATH = "	Asset/Data/Prefab/";

	nlohmann::json JsonData;
};