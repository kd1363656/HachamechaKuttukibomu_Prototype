#pragma once

class Prefab;

class ResourceManager
{
public:

	ResourceManager () = default;
	~ResourceManager() = default;

	void Init();
	
private:

	std::unordered_map<std::string , std::shared_ptr<Prefab>> m_prefabList;

};