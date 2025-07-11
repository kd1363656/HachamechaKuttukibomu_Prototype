#pragma once
#include "../FileIOBase.h"

class GameObjectFileIO : public FileIOBase
{
public:

	GameObjectFileIO ()         = default;
	~GameObjectFileIO()override = default;

	void Init()override;

	void SaveSceneData() override;
	void LoadSceneData() override;

	void SavePrefabData();
	void LoadPrefabData(const std::string& TypeName, const std::string& LocalPath);

private:

	const std::string FIRST_FILE_PATH              = "Asset/Data/GameObject/GameScene/GameObject";
	const std::string COMMON_PREFAB_DIRECTORY_PATH = "Asset/Data/Prefab/";

	void LoadGameObjectData(std::string&& ClassName , const nlohmann::json& Json = nlohmann::json());
};