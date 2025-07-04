﻿#include "ImGuiManager.h"

#include "../../main.h"

#include "../../GameObject/Camera/CameraBase.h"

#include "../../GameObject/Actor/ActorBase.h"

#include "../../Factory/Factory.h"

#include "../../Scene/SceneManager.h"
#include "../../Scene/BaseScene/BaseScene.h"

#include "../InputManager/RawInputManager.h"

void ImGuiManager::Init()
{
	GenereateGameObjectNameFilter<CameraBase>();
	GenereateGameObjectNameFilter<ActorBase> ();
}

void ImGuiManager::Update()
{
	//ImGui::ShowDemoWindow();
	DrawProjectPanel();

	DrawWorldSettingPanel();

	DrawUserInputInfoPanel();

	DrawFactoryPanel     ();
	DrawInspector        ();

	DrawPopups();
}

void ImGuiManager::DrawProjectPanel()
{
	auto& scene_ = SceneManager::GetInstance();

	if (!scene_.GetGameObjectFileIO())return;

	if (ImGui::Begin("Project" , nullptr , ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Save"))
				{
					scene_.GetGameObjectFileIO()->SaveSceneData();

					// セーブ完了モーダルポップアップの描画
					m_isShowSavePopUp = true;
					
				}
				if(ImGui::MenuItem("Load"))
				{
					// TODO
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	ImGui::End();
}

void ImGuiManager::DrawUserInputInfoPanel()
{
	auto& input_ = RawInputManager::GetInstance();

	if(ImGui::Begin("UserInputInfoPanel"))
	{
		ImGui::Separator();
		if(ImGui::TreeNode("KeyInfo"))
		{
			if(ImGui::BeginChild(ImGui::GetID((void*)0) , ImVec2(260.0f , 100.0f) , ImGuiWindowFlags_NoTitleBar))
			{
				for(auto& [key_ , value_] : input_.GetKeyStateList())
				{
					ImGui::Text("Virtual Key Codes %d : State %s",  key_ , JudgeInputStatus(value_));
				}
			}
			ImGui::EndChild();

			ImGui::TreePop();
		}

		ImGui::Separator();
		if(ImGui::TreeNode("MouseInfo"))
		{
			Mouse::Data mouseData_ = input_.GetMouseData();
			
			ImGui::Separator();
			ImGui::Text("Mouse LocationX           : %.2f" , mouseData_.location.x);
			ImGui::Text("Mouse LocationY           : %.2f" , mouseData_.location.y);
			ImGui::Separator();
			ImGui::Text("Mouse Click Middle Status : %s"   , JudgeInputStatus(mouseData_.isClickMiddle));
			ImGui::Text("Mouse Click Left   Status : %s"   , JudgeInputStatus(mouseData_.isClickLeft  ));
			ImGui::Text("Mouse Click Right  Status : %s"   , JudgeInputStatus(mouseData_.isClickRight ));

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void ImGuiManager::DrawWorldSettingPanel()
{
	if (ImGui::Begin("WorldSettingPanel"))
	{
		if (ImGui::TreeNode("TimeDilation"))
		{
			ImGui::DragFloat("TimeScale", Application::Instance().GetTimeScalePtr(), 0.1f, 0.0f, 1.0f);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void ImGuiManager::DrawFactoryPanel()
{
	if (ImGui::Begin("FactoryPanel"))
	{
		DrawFactoryPanel(GameObjectID::GetTypeID<ActorBase >() , "Add Actor" );
		ImGui::Separator();
		DrawFactoryPanel(GameObjectID::GetTypeID<CameraBase>() , "Add Camera");
		ImGui::Separator();
	}
	ImGui::End();
}

void ImGuiManager::DrawFactoryPanel(uint32_t BaseTypeID, const char* WidgetLabel)
{
	// まず登録した基底クラスの"ID"が見つかるかどうかを探す
	auto itr_ = m_gameObjectNameFilter.find(BaseTypeID);

	if (itr_ != m_gameObjectNameFilter.end())
	{
		std::string baseTypeName = itr_->second;

		// 見つかったらその基底クラスを含んだ派生クラスをファクトリーのコンボボックスに表示されるようにする
		if (ImGui::TreeNode(baseTypeName.data()))
		{
			// 基底クラスの"ID"を送りその基底クラスの"ID"とファクトリーが保持している
			// "ID"が一致すれば基底クラスを継承した派生クラスなのでウェジットに表示する
			DrawClassSelectorDropdown(itr_->first);
			DrawCreateButton         (WidgetLabel);
			ImGui::TreePop();
		}
	}
}

void ImGuiManager::DrawClassSelectorDropdown(uint32_t BaseTypeID)
{
	auto& factory_ = Factory::GetInstance();

	// "ImGui::BeginCombo"は第一引数に何か文字列を入れないとしっかり動作してくれないことが分かった
	if (ImGui::BeginCombo("## Hidden", m_createObjectName.c_str()))
	{
		for(const auto& [key_ , value_] : factory_.GetGameObjectFactoryMethodList())
		{
			// 最終の基底クラスの"ID"と一致しなければ処理を飛ばす
			if (value_.finalBaseTypeID != BaseTypeID)continue;
		
			const char* className_ = key_.data();

			// ファクトリーに登録されている名前と今選択している名前が一致しているかどうかを確認
			bool isSelected_ = (m_createObjectName == className_);

			// ImGui::Selectableはユーザーが項目をクリックしたときだけ"true"を返す
			if (ImGui::Selectable(className_ , isSelected_))
			{
				m_createObjectName = className_;
			}

			if (isSelected_)
			{
				// 選ばれている項目にフォーカスする(カーソルが合うように)
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
}

void ImGuiManager::DrawCreateButton(const char* WidgetLabel)
{
	auto& sceneManager_ = SceneManager::GetInstance();
	auto& factory_      = Factory::GetInstance  ();

	if (auto scene_ = sceneManager_.GetCurrentScene().lock())
	{
		// ウィジェットの位置を改行せずに離す形で配置する
		ImGui::SameLine(260.0f);

		if (ImGui::Button(WidgetLabel))
		{
			// しっかりとファクトリーに登録されているか確認
			if (auto itr_      = factory_.GetGameObjectFactoryMethodList().find(m_createObjectName);
				auto instance_ = itr_->second.gameObjectFactoryMethod()							  )
			{
				// 初期化してから渡す
				instance_->Init();

				std::string log_ = "Add ";
				log_ += instance_->GetTypeName().data();
				log_ += "\n";

				KdDebugGUI::Instance().AddLog(log_.data());

				scene_->AddObject(instance_);
			}
		}
	}
}

void ImGuiManager::DrawInspector()
{
	auto scene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!scene_)return;

	if (ImGui::Begin("Inspector"))
	{
		DrawInspector(GameObjectID::GetTypeID<CameraBase>());
		ImGui::Separator();
		DrawInspector(GameObjectID::GetTypeID<ActorBase> ());
		ImGui::Separator();
	}
	ImGui::End();
}

void ImGuiManager::DrawInspector(uint32_t BaseTypeID)
{
	// この関数を使っている大本の関数でヌルチェックを行っているがもしかしたら別の関数で使う可能性があるので
	// ここでもヌルチェックを行う
	auto scene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!scene_)return;

	auto itr_ = m_gameObjectNameFilter.find(BaseTypeID);

	if (itr_ != m_gameObjectNameFilter.end())
	{
		std::string baseTypeName = itr_->second;

		if (ImGui::TreeNode(baseTypeName.data()))
		{
			for (auto obj_ : scene_->GetObjectList())
			{
				if (obj_->GetFinalBaseTypeID() == BaseTypeID)
				{

					std::string className_ = obj_->GetTypeName().data();
					// ウィジェットの"TreeNode"が個別に識別できるようにポインターを刷り込ませる
					className_ += "##" + std::to_string(reinterpret_cast<uintptr_t>(obj_.get()));

					if (ImGui::TreeNode(className_.data()))
					{
						ImGui::SameLine(200.0, 0.0f);

						// ボタンが押されたら対象のオブジェクトを削除
						if (ImGui::SmallButton("Delete"))
						{
							obj_->SetIsExpired(true);
						}

						DrawSeparate();
						ImGui::Text("Transform");
						obj_->ImGuiTransformInspector();
						DrawSeparate();
						ImGui::Text("Material");
						obj_->ImGuiMaterialInspector();

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
	}
}

void ImGuiManager::DrawSeparate()
{
	ImGui::Spacing  ();
	ImGui::Separator();
	ImGui::Spacing  ();
}

void ImGuiManager::DrawPopups()
{
	if (m_isShowSavePopUp)
	{
		ImGui::OpenPopup("Save Completed");
		m_isShowSavePopUp = false;
	}

	// セーブ完了モーダルポップアップの描画
	if (ImGui::BeginPopupModal("Save Completed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button("/=========Ok==========/"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}