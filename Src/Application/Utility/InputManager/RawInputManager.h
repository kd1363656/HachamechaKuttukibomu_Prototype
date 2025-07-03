#pragma once

#include "../SingletonBase.h"

#include <Xinput.h>
#pragma comment(lib , "xinput.lib")	//XInputライブラリリンク

namespace Mouse
{
	struct Data
	{
		Math::Vector2 pos;
		int  wheelDelta;
		bool isClickMiddle;
		bool isReleaseMiddle;
		bool isClickRight;
		bool isClickLeft;

		Mouse::Data() :
			pos			   (Math::Vector2::Zero) , 
			wheelDelta	   (0)					 ,
			isClickMiddle  (false)				 ,
			isReleaseMiddle(false)				 ,
			isClickRight   (false)				 ,
			isClickLeft    (false)				
		{}
	};
}

// キーボードの入力は保証
class RawInputManager : public SingletonBase<RawInputManager>
{
public:

	RawInputManager () = default;
	~RawInputManager() = default;

	void SetHWnd(HWND Set) { m_hWnd = Set; }

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

	// キー入力状態を取得する
	bool GetKeyState(int VKey)
	{
		auto itr_ = m_keyStateList.find(VKey);
		return (itr_ != m_keyStateList.end() && itr_->second);
	}

	Mouse::Data GetMouseData() { return m_mouseData; }

	void RegisterDevice();
	void ProcessInput  (LPARAM LParam);
	void Vibrate       (float LeftMotor , float RightMotor , int Duration);

private:

	HWND m_hWnd = NULL;

	// キーの入力状態を毎フレーム格納するリスト
	std::unordered_map<int, bool> m_keyStateList;
	// マウスの入力状態、座標を舞フレーム格納
	Mouse::Data m_mouseData;

	std::function<void(int   Key    , bool  Presseed )> m_keyboardCallback;		//キーボード用コールバック
	std::function<void(Mouse::Data MouseData         )> m_mouseCallback;		//マウス用
	std::function<void(int   Button , bool  Pressed  )> m_gamepadCallback;		//ゲームパッド用コールバック
	std::function<void(float X      , float Y        )> m_gamepadStickCallback;	//ゲームパッドのスティック用のコールバック
};