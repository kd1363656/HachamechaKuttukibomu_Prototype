#pragma once
#include "../../Utility/SingletonBase.h"
#include "PrefabData.h"

class PrefabData;

class PrefabManager : public SingletonBase<PrefabManager>
{
public:

	PrefabManager () = default;
	~PrefabManager() = default;

	std::weak_ptr<PrefabData> GetPrefabData(const std::string& Key);

	void AddPrefabData(const std::string& Key);

private:

	std::unordered_map<std::string, std::shared_ptr<PrefabData>> m_prefabDataList;
};