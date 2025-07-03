#pragma once
#include "../Struct.h"
#include "../SingletonBase.h"

// 発表会、瞬発的なデバッグで使うのでシングルトンで管理
class DebugKey : public SingletonBase<DebugKey>
{
public:

	DebugKey () = default;
	~DebugKey() = default;

	void Init  ();
	void Update();

	const Tag::KeyToggleInfo& GetKeyToggle(std::string KeyName)const;
	Tag::KeyToggleInfo& GetKeyToggle(std::string KeyName);

	void SetIsActive(std::string KeyName , bool Set)
	{
		GetKeyToggle(KeyName).isActive = Set;
	}

private:

	
	void AddKeyToggle(std::string KeyName , int KeyCode);
	
	void HandleDebugToggleKey(Tag::KeyToggleInfo& Toggle);

	bool IsDebugKeyActivated();

	std::unordered_map<std::string , Tag::KeyToggleInfo> m_keyState;
};