#pragma once

#include "../Src/Application/Utility/SingletonBase.h"

#include <Xinput.h>
#pragma comment(lib , "xinput.lib")	//XInputライブラリリンク

namespace Mouse
{
	struct Data
	{
		Math::Vector2 location        = Math::Vector2::Zero;
		int           wheelDelta      = 0;
		bool          isClickMiddle   = false;
		bool          isReleaseMiddle = false;
		bool          isClickRight    = false;	
		bool          isClickLeft     = false;
	};
}

// キーボードの入力は保証
class RawInputManager : public SingletonBase<RawInputManager>
{
public:

	RawInputManager () = default;
	~RawInputManager() = default;

	void ResetKeyStates();

	void SetHWnd(HWND Set) { m_hWnd = Set; }

	void SetNowMouseLocation(Math::Vector2 Set) { m_nowMouseData.location = Set; }

	const std::unordered_map<int , bool>& GetNowKeyStateList()const { return m_nowKeyStateList; }
	const std::unordered_map<int , bool>& GetOldKeyStateList()const { return m_oldKeyStateList; }

	// キー入力状態を取得する
	bool GetKeyState(int VKey) const
	{
		auto itr_ = m_nowKeyStateList.find(VKey);
		if (itr_ != m_nowKeyStateList.end()) 
		{
			return itr_->second;
		};

		return false;
	}

	// 一回だけ押されたか確認する関数
	bool IsKeyPressedOnce(int VKey) const
	{
		auto nowItr_ = m_nowKeyStateList.find(VKey);
		auto oldItr_ = m_oldKeyStateList.find(VKey);

		// もしキーが存在しなければ"false"
		if(nowItr_ == m_nowKeyStateList.end() ||
		   oldItr_ == m_oldKeyStateList.end()) 
		{
			return false;
		}

		return nowItr_->second && !oldItr_->second;
	}

	bool IsMouseClickedLeftOnce() const
	{
		return m_nowMouseData.isClickLeft && !m_oldMouseData.isClickLeft;
	}

	bool IsMouseClickedRightOnce() const
	{
		return m_nowMouseData.isClickRight && !m_oldMouseData.isClickRight;
	}

	bool IsMouseClickedMiddleOnce() const
	{
		return m_nowMouseData.isClickMiddle && !m_oldMouseData.isClickMiddle;
	}

	// キーボードのコールバック設定関数
	void SetKeyboardCallback(std::function<void(int Key , bool Pressed)> callback)
	{
		m_keyboardCallback = callback;
	}

	// マウスのコールバック
	void SetMouseCallback(std::function<void(Mouse::Data)> callback)
	{
		m_mouseCallback = callback;
	}

	// ゲームパッド用のコールバック
	void SetGamepadCallback(std::function<void(int Button , bool Pressed)> callback)
	{
		m_gamepadCallback = callback;
	}

	// ゲームパッドスティック用のコールバック
	void SetGamepadStickCallback(std::function<void(float x , float y)> callback)
	{
		m_gamepadStickCallback = callback;
	}

	Mouse::Data GetMouseData() { return m_nowMouseData; }

	void RegisterDevice();
	void ProcessInput  (LPARAM LParam);
	void Vibrate       (float LeftMotor , float RightMotor , int Duration);

	void BackUpInputState();

private:

	HWND m_hWnd = NULL;

	// マルチスレッド対応用
	// すべての入力で共通の"mutex"
	std::mutex m_inputMutex;

	// キーの入力状態を毎フレーム格納するリスト
	std::unordered_map<int, bool> m_nowKeyStateList;
	std::unordered_map<int, bool> m_oldKeyStateList;

	// マウスの入力状態、座標を舞フレーム格納
	Mouse::Data m_nowMouseData;
	Mouse::Data m_oldMouseData;

	std::function<void(int   Key    , bool  Presseed )> m_keyboardCallback;		//キーボード用コールバック
	std::function<void(Mouse::Data MouseData         )> m_mouseCallback;		//マウス用
	std::function<void(int   Button , bool  Pressed  )> m_gamepadCallback;		//ゲームパッド用コールバック
	std::function<void(float X      , float Y        )> m_gamepadStickCallback;	//ゲームパッドのスティック用のコールバック
};