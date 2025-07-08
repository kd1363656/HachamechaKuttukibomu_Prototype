#include "../../Application/main.h"

#include "KdDebugGUI.h"

#include "Src/Application/Utility/ImGui/ImGuiManager.h"

#include "Src/Application//Utility/InputManager/RawInputManager.h"

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
	ImGui::StyleColorsDark();

	// "ImGuiDocking"
	ImGuiIO& io = ImGui::GetIO();
	// ここの部分でImGuiのドッキング機能を有効にする
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      //    マルチウィンドウを有効にする
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        //    ドッキングを有効にする

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

void KdDebugGUI::GuiRelease()
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	m_uqLog = nullptr;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
