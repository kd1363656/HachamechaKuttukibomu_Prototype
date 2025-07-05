#pragma once

class FileIOBase
{
public:

	FileIOBase         () = default;
	virtual ~FileIOBase() = default;

	virtual void Init() { /* まだ実装されていません、派生クラスで実装してください */ }

	virtual void SaveSceneData() { /* まだ実装されていません、派生クラスで実装してください */ }
	virtual void LoadSceneData() { /* まだ実装されていません、派生クラスで実装してください */ }

	std::string_view GetFilePath() { return m_filePath; }

	void SetFilePath(std::string& Set) { m_filePath = Set; }

protected:

	std::string m_filePath;
};