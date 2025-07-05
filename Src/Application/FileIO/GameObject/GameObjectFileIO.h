#pragma once
#include "../FileIOBase.h"

class GameObjectFileIO: public FileIOBase
{
public:

	GameObjectFileIO         () = default;
	virtual ~GameObjectFileIO() = default;

	void Init()override;

	void SaveSceneData() override;
	void LoadSceneData() override;

private:

	// TODO
	std::string FIRST_FILE_PATH = "Asset/Data/SceneData/GameScene/GameScene";

	void LoadGameObjectData(std::string&& ClassName , const nlohmann::json& Json = nlohmann::json());
};