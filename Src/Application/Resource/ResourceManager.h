#pragma once

class Prefab;

// 実装する機能、保存位置が定位置なためここにファイル保存機能を持たせておく
// もし複雑化したらクラスを増やして責任を分離
// とりあえずプレハブのみ実装
class ResourceManager
{
public:

	ResourceManager () = default;
	~ResourceManager() = default;

	auto& GetPrefabDataList() { return m_prefabDataList; }

	std::weak_ptr<Prefab> GetPrefabData(const std::string& TypeName);

private:

	std::unordered_map<std::string , std::shared_ptr<Prefab>> m_prefabDataList;
};