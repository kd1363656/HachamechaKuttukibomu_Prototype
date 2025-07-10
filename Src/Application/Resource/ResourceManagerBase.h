#pragma once
#include "Prefab/PrefabManager.h"

class ResourceManagerBase
{
public:

	ResourceManagerBase         () = default;
	virtual ~ResourceManagerBase() = default;

	virtual void Init() {/* まだ実装されていません、派生クラスで実装してください */ }

private:

	std::unique_ptr<PrefabManager> m_prefabManager = nullptr;
};