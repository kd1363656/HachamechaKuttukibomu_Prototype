#pragma once

class Prefab
{
public:

	Prefab () = default;
	~Prefab() = default;

	const nlohmann::json& GetJsonData()const { return m_jsonData; }

	void SetJsonData    (const nlohmann::json& Set) { m_jsonData     = Set; }
	void SetSaveFilePath(const std::string&    Set) { m_saveFilePath = Set; }

private:

	nlohmann::json m_jsonData;
	
	std::string m_saveFilePath;
};