#ifdef _DEBUG
#include "ImGuiManager.h"

#include "../../main.h"

void ImGuiManager::Init()
{
	// TODO
}

void ImGuiManager::Update()
{
	// デバッグウィンドウ(日本語を表示したい場合はこう書く)
	if (ImGui::Begin(U8("WorldSetting")))
	{
		if(ImGui::TreeNode("TimeDilation"))
		{
			ImGui::DragFloat("TimeScale" , Application::Instance().GetTimeScalePtr() , 0.1f , 0.0f , 1.0f);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}
#endif