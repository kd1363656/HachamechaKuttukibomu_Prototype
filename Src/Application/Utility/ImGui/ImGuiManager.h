#pragma once

#include "../SingletonBase.h"

class ImGuiManager : public SingletonBase<ImGuiManager>
{
public:

	ImGuiManager ()         = default;
	~ImGuiManager()override = default;

	void Init  ();
	void Update();

	// 同じ関数を"Factory.h"でも使っているが"StringUtility.h"を作ってまで共通化するにしては使う場面が少ないし大げさ
	// だからコピ-したものを使っています
	void StripClassPrefix(std::string& ClassName);

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

		StripClassPrefix(className_);

		m_gameObjectNameFilter.emplace(id_ , className_);
	}

private:

	void WorldSettingInspector();
	void FactoryInspector     ();

	void SelectCreateObjectInspector (uint32_t BaseTypeID);

	void CreateSelectedObjectInspector();

	std::string m_createObjectName = "Player";

	std::unordered_map<uint32_t , std::string> m_gameObjectNameFilter;
};