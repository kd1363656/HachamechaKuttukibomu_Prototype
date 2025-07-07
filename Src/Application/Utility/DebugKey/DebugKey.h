#pragma once
#include "../CommonStruct.h"
#include "../SingletonBase.h"

// 発表会、瞬発的なデバッグで使うのでシングルトンで管理
class DebugKey : public SingletonBase<DebugKey>
{
public:

	DebugKey () = default;
	~DebugKey() = default;

	void Init  ();
	void Update();

	const CommonStruct::KeyToggleInfo& GetKeyToggle(std::string KeyName)const;
	CommonStruct::KeyToggleInfo&       GetKeyToggle(std::string KeyName);

	void SetIsActive(std::string KeyName , bool Set)
	{
		GetKeyToggle(KeyName).isActive = Set;
	}

private:

	
	void AddKeyToggle(std::string KeyName , int KeyCode);
	
	void HandleDebugToggleKey(CommonStruct::KeyToggleInfo& Toggle);

	bool IsDebugKeyActivated();

	std::unordered_map<std::string , CommonStruct::KeyToggleInfo> m_keyState;
};