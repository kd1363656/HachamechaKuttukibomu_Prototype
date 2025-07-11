#include "ResourceManager.h"

#include "Prefab/Prefab.h"

#include "../../System/FileSystem/FileSystem.h"

std::weak_ptr<Prefab> ResourceManager::GetPrefabData(const std::string& TypeName)
{
	if (!m_prefabDataList.contains(TypeName)) { return std::weak_ptr<Prefab>(); }

	return m_prefabDataList[TypeName];
}