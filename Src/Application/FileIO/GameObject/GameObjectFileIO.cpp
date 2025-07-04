﻿#include "GameObjectFileIO.h"

#include "../../Scene/SceneManager.h"
#include "../../Scene/BaseScene/BaseScene.h"

#include "../../Utility/JsonUtility.h"

#include "../../Factory/Factory.h"

#include "../../GameObject/Actor/ActorBase.h"

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
		json_.emplace_back(obj_->SaveJsonData()); 	
	}

	JsonUtility::SaveJsonFile(json_, m_filePath);
}

void GameObjectFileIO::LoadSceneData()
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!currentScene_) { return; }

	nlohmann::json json_ = JsonUtility::LoadJsonFile(m_filePath);

	for(auto it_ : json_)
	{
		LoadGameObjectData(it_.value("TypeName", ""), it_);
	}
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

		// "Json"データの"Null"チェック
		// 必要ならクラス名チェックも入れておいた方がいい
		if (!Json.is_null()) { instance_->LoadJsonData(Json); }

		currentScene_->AddObject(instance_);
	}
}