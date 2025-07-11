#include "GameObjectFileIO.h"

#include "../../Factory/Factory.h"

#include "../../GameObject/Actor/ActorBase.h"

#include "../../Scene/BaseScene/BaseScene.h"
#include "../../Scene/SceneManager.h"

#include "../../Resource/ResourceManager.h"
#include "../../Resource/Prefab/Prefab.h"

#include "../../../System/FileSystem/FileSystem.h"

void GameObjectFileIO::Init()
{
	m_filePath = FIRST_FILE_PATH;

	LoadSceneData();
}

void GameObjectFileIO::SaveSceneData()
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!currentScene_) { return; }

	nlohmann::json json_ = nlohmann::json::array();

	// すべてのオブジェクトを取得し状態を保持
	for(auto& obj_ : currentScene_->GetObjectList())
	{		
		json_.emplace_back(obj_->SaveTransformData()); 	
	}

	FileSystem::SaveJsonFile(json_, m_filePath);
}

void GameObjectFileIO::LoadSceneData()
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!currentScene_) { return; }

	nlohmann::json json_ = FileSystem::LoadJsonFile(m_filePath);

	for(auto it_ : json_)
	{
		LoadGameObjectData(it_.value("TypeName", ""), it_);
	}

	// "Json"を読み込んだ後にすべき処理を行う
	for(auto& obj_ : currentScene_->GetObjectList())
	{
		obj_->PostLoadInit();
	}
}

void GameObjectFileIO::SavePrefabData()
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!currentScene_) { return; }

	// TODO
	// プレハブ用のデータをここで保存
	if(auto& resourceManager_ = currentScene_->GetResourceManager())
	{
		for(auto& [key_ , value_] : resourceManager_->GetPrefabDataList())
		{
			if(auto prefabData_ = resourceManager_->GetPrefabData(key_).lock())
			{
				FileSystem::SaveJsonFile(prefabData_->GetJsonData() , prefabData_->GetSaveFilePath().data());
			}
		}
	}
}

void GameObjectFileIO::LoadPrefabData(const std::string& TypeName, const std::string& LocalPath)
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!currentScene_) { return; }

	auto& resourceManager_ = currentScene_->GetResourceManager();
	
	if (!resourceManager_) { return; }

	// もし一回でもプレハブを作製していたらする必要がないので"return"
	if (resourceManager_->GetPrefabDataList().contains(TypeName)) { return; }

	// 生成したプレハブにに"移動速度"などのパラメータを含む"Json"ファイルをロードしてもらう
	const std::string    filePath_ = COMMON_PREFAB_DIRECTORY_PATH + LocalPath + TypeName + FileSystem::SLASH_SYMBOL + TypeName;
	const nlohmann::json json_     = FileSystem::LoadJsonFile(filePath_);

	auto instance_ = std::make_shared<Prefab>();
	instance_->SetJsonData(json_);

	// セーブするときに使うファイルパスを保存
	instance_->SetSaveFilePath(filePath_);
	
#ifdef _DEBUG

	// プレハブ操作用のオブジェクトを作って格納
	std::shared_ptr<KdGameObject> prefabObject_ = std::make_shared<KdGameObject>();
	prefabObject_->Init();

	instance_->SetPrefabPreviewObject(prefabObject_);

	std::string typeName_ = TypeName;
	KdDebugGUI::Instance().AddLog("\nPrehab : %s is successfully Load" , typeName_.c_str());
#endif
	resourceManager_->GetPrefabDataList().emplace(TypeName , instance_);
}

void GameObjectFileIO::LoadGameObjectData(std::string&& ClassName, const nlohmann::json& Json)
{
	auto& factory_      = Factory::GetInstance                              ();
	auto  currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!currentScene_) { return; }

	auto itr_ = factory_.GetGameObjectFactoryMethodList().find(ClassName);

	if (itr_ != factory_.GetGameObjectFactoryMethodList().end())
	{
		// キーに対応したクラスの生成を行う
		std::shared_ptr<KdGameObject> instance_ = itr_->second.gameObjectFactoryMethod();
		instance_->Init();

		LoadPrefabData(instance_->GetTypeName().data(), instance_->GetPrefabSavePath().data());

		if(auto& resourceManager_ = currentScene_->GetResourceManager())
		{
			// ロードしたプレハブ情報をオブジェクトに付与
			auto prefabData_ = resourceManager_->GetPrefabData(instance_->GetTypeName().data()).lock();

			if(prefabData_)
			{
				instance_->LoadPrefabData(prefabData_->GetJsonData());
			}
		}

		// "Json"データの"Null"チェック
		// 必要ならクラス名チェックも入れておいた方がいい
		if (!Json.is_null()) 
		{
			instance_->LoadTransformData(Json); 
		}

		currentScene_->AddObject(instance_);
	}
}