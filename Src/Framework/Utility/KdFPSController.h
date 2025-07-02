#pragma once

//=======================================================================
// アプリケーションのFPS制御 + 測定
// ゲーム内の全オブジェクトがフレームレートに依存しなくていいようになる
// 演出面でも拡張性が高くなる、だから実装しました
//=======================================================================

class KdFPSController
{
public:

	KdFPSController () = default;
	~KdFPSController() = default;

	void Init           ();
	void UpdateStartTime();
	void Update         ();

	float  GetDeltaTime      ()const { return m_deltaTime;       }
	float* GetDeltaTime      ()      { return &m_deltaTime;      }
	float  GetScaledDeltaTime()const { return m_scaledDeltaTime; }
	
	float  GetTimeScale()const { return m_timeScale;  }
	float* GetTimeScale()      { return &m_timeScale; }

	int GetMaxFps()const { return m_maxFps; }
	int GetNowFps()const { return m_nowFps; }

	void SetTimeScale(float Set) { m_timeScale = Set; }

private:

	std::chrono::steady_clock::time_point m_previousTime   = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point m_frameBeginTime = std::chrono::steady_clock::now();

	float m_deltaTime       = 0.0f;
	float m_timeScale       = 0.0f;
	float m_scaledDeltaTime = 0.0f;

	int m_maxFps = 0;
	int m_nowFps = 0;

	void Control   ();
	void Monitoring();
};
