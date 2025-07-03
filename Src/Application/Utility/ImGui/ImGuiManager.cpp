#include "ImGuiManager.h"

#include "../../main.h"

#include "../../GameObject/Camera/CameraBase.h"

#include "../../GameObject/Actor/ActorBase.h"

#include "../../Factory/Factory.h"

#include "../../Scene/SceneManager.h"
#include "../../Scene/BaseScene/BaseScene.h"

void ImGuiManager::Init()
{
	GenereateGameObjectNameFilter<CameraBase>();
	GenereateGameObjectNameFilter<ActorBase> ();
}

void ImGuiManager::Update()
{
	//ImGui::ShowDemoWindow();

	DrawWorldSettingPanel ();
	DrawFactoryPanel      ();
	DrawTransformInspector();
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
		DrawFactoryPanel(GameObjectID::GetTypeID<CameraBase>() , "Add Camera");
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
		ImGui::SameLine(350.0f);

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

				KdDebugGUI::Instance().AddLog(log_.data());

				scene_->AddObject(instance_);
			}
		}
	}
}

void ImGuiManager::DrawTransformInspector()
{
	auto scene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!scene_)return;

	if (ImGui::Begin("TransformInspector"))
	{
		for (auto cache_ : scene_->GetCacheObjectList<ActorBase>())
		{
			if (auto wp_ = cache_.lock())
			{
				std::string className_ = wp_->GetTypeName().data();
				// ウィジェットの"TreeNode"が個別に識別できるようにポインターを刷り込ませる
				className_ += "##" + std::to_string(reinterpret_cast<uintptr_t>(wp_.get()));

				if(ImGui::TreeNode(className_.data()))
				{
					wp_->ImGuiInspector();
					ImGui::TreePop();
				}
			}
		}
	}
	ImGui::End();
}