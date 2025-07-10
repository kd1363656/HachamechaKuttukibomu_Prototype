#include "../../Application/main.h"

#include "KdDebugGUI.h"

#include "Src/System/ImGui/ImGuiManager.h"
#include "Src/System/Input/RawInputManager.h"

KdDebugGUI::KdDebugGUI()
{}
KdDebugGUI::~KdDebugGUI()
{ 
	GuiRelease(); 
}

void KdDebugGUI::GuiInit()
{
	// 初期化済みなら動作させない
	if (m_uqLog) return;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style

	// "ImGuiDocking"
	ImGuiIO& io = ImGui::GetIO();
	// ここの部分でImGuiのドッキング機能を有効にする
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      //    マルチウィンドウを有効にする
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        //    ドッキングを有効にする

	ImGui::StyleColorsDark();

	//ImGui::StyleColorsClassic();
	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(Application::Instance().GetWindowHandle());
	ImGui_ImplDX11_Init(
		KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext());

#include "imgui/ja_glyph_ranges.h"
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	// 日本語対応
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);
	
	m_uqLog = std::make_unique<ImGuiAppLog>();

	m_canShowDebugWindow    = true;
	m_isHeldShowDebugWindow = false;
}

void KdDebugGUI::GuiProcess()
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	//IdleBySleeping(m_fpsIdle);

	//===========================================================
	// ImGui開始
	//===========================================================
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ログを描画するかしないかを決める操作
	auto& input_ = RawInputManager::GetInstance();

	if (input_.GetKeyState(VK_CONTROL))
	{
		if (input_.GetKeyState(VK_RETURN))
		{
			if (!m_isHeldShowDebugWindow)
			{
				if (m_canShowDebugWindow)
				{
					m_canShowDebugWindow = false;
				}
				else
				{
					m_canShowDebugWindow = true;
				}
				m_isHeldShowDebugWindow = true;
			}
		}
	}
	else
	{
		m_isHeldShowDebugWindow = false;
	}

	//===========================================================
	// 以下にImGui描画処理を記述
	//===========================================================
	if (m_canShowDebugWindow)
	{
		ImGuiManager::GetInstance().Update();

		// ログウィンドウ
		m_uqLog->Draw("Log Window");
	}

	//=====================================================
	// ログ出力 ・・・ AddLog("～") で追加
	//=====================================================

//	m_uqLog->AddLog("hello world\n");

	//=====================================================
	// 別ソースファイルからログを出力する場合
	//=====================================================

//	KdDebugGUI::Instance().AddLog("TestLog\n");

	//===========================================================
	// ここより上にImGuiの描画はする事
	//===========================================================
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// マルチウィンドウを有効にしている場合は、以下の処理も必要
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
	    // 描画、更新
	    ImGui::UpdatePlatformWindows();
	    ImGui::RenderPlatformWindowsDefault();
	
	    // ここでレンダリングターゲットを戻す必要がある
	    KdDirect3D::Instance().WorkDevContext()->OMSetRenderTargets(
	        1,
	        KdDirect3D::Instance().GetBackBuffer()->WorkRTViewAddress(),
	        KdDirect3D::Instance().GetZBuffer()->WorkDSView());
	}
}

void KdDebugGUI::AddLog(const char* fmt,...)
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	va_list args;
	va_start(args, fmt);
	m_uqLog->AddLog(fmt);
	va_end(args);
}

void KdDebugGUI::IdleBySleeping(FpsIdling& IoIdling)
{
	// TODO
	IoIdling.isIdling = false;
	if ((IoIdling.fpsIdle > 0.0f) && IoIdling.enableIdling)
	{
		// スリープ前の時刻
		std::chrono::steady_clock::time_point  beforeWait_  = std::chrono::steady_clock::now();
		float								   waitTimeout_ = 1.0 / IoIdling.fpsIdle;

		// イベント待ち＋タイムアウト
		WaitForEventTimeout(waitTimeout_);

		// スリープ後の時刻
		std::chrono::steady_clock::time_point afterWait_        = std::chrono::steady_clock::now();
		std::chrono::duration<float>          waitDuration_     = afterWait_ - beforeWait_;
		float                                 waitIdleExpected_ = 1.0 / IoIdling.fpsIdle;

		IoIdling.isIdling = (waitDuration_.count() > waitIdleExpected_ * 0.9);
	}
}

void KdDebugGUI::GuiRelease()
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	m_uqLog = nullptr;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void KdDebugGUI::WaitForEventTimeout(float TimeoutSeconds)
{
	DWORD timeoutMS_ = static_cast<DWORD>(TimeoutSeconds * 1000.0);
	// QS_ALLINPUT: すべての入力メッセージを対象
	MsgWaitForMultipleObjects(0, nullptr, FALSE, timeoutMS_, QS_ALLINPUT);
}