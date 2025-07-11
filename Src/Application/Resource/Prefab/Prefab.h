#pragma once

class Prefab
{
public:

	Prefab () = default;
	~Prefab() = default;

	void SetJsonData    (const nlohmann::json& Set) { m_jsonData = Set; }

	void SetSaveFilePath(const std::string&    Set) { m_saveFilePath = Set; }

	const nlohmann::json& GetJsonData()const { return m_jsonData; }

	std::string_view GetSaveFilePath() { return m_saveFilePath; }
	
private:

	nlohmann::json m_jsonData;
	
	std::string m_saveFilePath;


#ifdef _DEBUG
public:

	void SetPrefabPreviewObject(std::shared_ptr<KdGameObject> Set) { m_prefabPreviewObject = Set; }

	std::weak_ptr<KdGameObject> GetPrefabPreviewObject() { return m_prefabPreviewObject; }

private:
	std::weak_ptr<KdGameObject> m_prefabPreviewObject;
#endif
};