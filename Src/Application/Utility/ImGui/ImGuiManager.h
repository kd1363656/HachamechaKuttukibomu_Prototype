﻿#pragma once
#include "../SingletonBase.h"
#include "../StringUtility.h"

class ImGuiManager : public SingletonBase<ImGuiManager>
{
public:

	ImGuiManager ()         = default;
	~ImGuiManager()override = default;

	void Init  ();
	void Update();

	// テンプレートの特殊化で生成した一意な"ID"とそのクラスの基底クラスの名前を結びつける
	template <class ObjectType>
	void GenereateGameObjectNameFilter()
	{
		const uint32_t id_ = GameObjectID::GetTypeID<ObjectType>();

		if (m_gameObjectNameFilter.contains(id_)) 
		{
			assert(false && "GameObjectNameFilterが重複登録されています、登録個所を確認してください");
			return;
		}

		std::string className_ = typeid(ObjectType).name();

		StringUtility::StripClassPrefix(className_);

		m_gameObjectNameFilter.emplace(id_ , className_);
	}

private:

	void DrawProjectPanel();

	void DrawUserInputInfoPanel();
	void DrawWorldSettingPanel ();

	void DrawFactoryPanel();
	void DrawFactoryPanel(uint32_t BaseTypeID , const char* WidgetLabel);

	void DrawClassSelectorDropdown(uint32_t BaseTypeID);
	void DrawCreateButton         (const char* WidgetLabel);

	void DrawInspector();
	void DrawInspector(uint32_t BaseTypeID);

	void DrawSeparate();

	void DrawPopups();

	const char* JudgeInputStatus(bool Status)
	{
		const char* keyStatus_ = "UnKnown";

		return keyStatus_ = Status ? "true" : "false";
	}

	// 対応するファクトリーごとに名前を変えていたら霧がないので一つだけ使用
	// これだけでも十分成り立つ
	std::string m_createObjectName = "Player";

	std::unordered_map<uint32_t , std::string> m_gameObjectNameFilter;

	bool m_isShowSavePopUp;
};