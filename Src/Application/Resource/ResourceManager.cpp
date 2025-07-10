#include "ResourceManager.h"

#include "Prefab/Prefab.h"

#include "../../System/FileSystem/FileSystem.h"

void ResourceManager::LoadPrefabData(const std::string& TypeName, const std::string& LocalPath)
{
	// もし一回でもプレハブを作製していたらする必要がないので"return"
	if (m_prefabDataList.contains(TypeName)) { return; }

	// 生成したプレハブにに"移動速度"などのパラメータを含む"Json"ファイルをロードしてもらう
	const std::string    filePath_ = COMMON_DIRECTORY_PATH + LocalPath + TypeName + FileSystem::SLASH_SYMBOL + TypeName;
	const nlohmann::json json_     = FileSystem::LoadJsonFile(filePath_);

	auto instance_ = std::make_shared<Prefab>();
	instance_->SetJsonData(json_);

	// セーブするときに使うファイルパスを保存
	instance_->SetSaveFilePath(filePath_);
	
#ifdef _DEBUG
	std::string typeName_ = TypeName;
	KdDebugGUI::Instance().AddLog("\nPrehab : %s is successfully Load" , typeName_.c_str());
#endif
	m_prefabDataList[TypeName] = instance_;
}

std::weak_ptr<Prefab> ResourceManager::GetPrefabData(const std::string& TypeName)
{
	if (!m_prefabDataList.contains(TypeName)) { return std::weak_ptr<Prefab>(); }

	return m_prefabDataList[TypeName];
}