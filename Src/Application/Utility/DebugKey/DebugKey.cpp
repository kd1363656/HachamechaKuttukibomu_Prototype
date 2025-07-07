#include "DebugKey.h"

#include "../../Scene/SceneManager.h"

#include "../InputManager/RawInputManager.h"

void DebugKey::Init()
{
	AddKeyToggle("PlayerInvincible" , 'I');
}

void DebugKey::Update()
{
	if (m_keyState.empty())return;

	for(auto& [key , value] : m_keyState)
	{
		HandleDebugToggleKey(value);
	}
}

const CommonStruct::KeyToggleInfo& DebugKey::GetKeyToggle(std::string KeyName) const
{
	auto itr_ = m_keyState.find(KeyName);

	if (itr_ != m_keyState.end())
	{
		return itr_->second;
	}
	
	assert(false && "デバックキーが登録されていません");
	static CommonStruct::KeyToggleInfo dummy_;	//フォールバック用
	return dummy_;

}

CommonStruct::KeyToggleInfo& DebugKey::GetKeyToggle(std::string KeyName)
{
	auto itr_ = m_keyState.find(KeyName);

	if (itr_ != m_keyState.end())
	{
		return itr_->second;
	}
	
	assert(false && "デバックキーが登録されていません");
	static CommonStruct::KeyToggleInfo dummy_;	//フォールバック用
	return dummy_;

}

void DebugKey::AddKeyToggle(std::string KeyName, int KeyCode)
{
	auto itr_ = m_keyState.find(KeyName);

	if (itr_ == m_keyState.end())
	{
		m_keyState.emplace(KeyName , CommonStruct::KeyToggleInfo(KeyCode) );
	}
	else
	{
		assert(false && "重複したデバックキーを登録しています");
	}
}

void DebugKey::HandleDebugToggleKey(CommonStruct::KeyToggleInfo& Toggle)
{
	auto& input_ = RawInputManager::GetInstance();

	if (IsDebugKeyActivated())
	{
		if (input_.GetKeyState(Toggle.keyCode))
		{
			if (!Toggle.isKeyHeld)
			{
				// 本来ならビットシフトのほうがいい
				Toggle.isActive = Toggle.isActive ? false : true;
			}

			Toggle.isKeyHeld = true;
		}
	}
	else
	{
		Toggle.isKeyHeld = false;
	}
}

// デバッグキーを実行するうえで必ず押さなければいけないキー
// 関数にすることで戻り値が一定になりタイプミスを減らし、条件を統一化する
bool DebugKey::IsDebugKeyActivated()
{
	auto& input_ = RawInputManager::GetInstance();
	return input_.GetKeyState('D');
}
