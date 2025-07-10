#pragma once
#include "../Utility/SingletonBase.h"
#include "../Utility/StringUtility.h"
#include "../Utility/CommonStruct.h"

class Factory : public SingletonBase<Factory>
{
public:

	Factory ()         = default;
	~Factory()override = default;

	void Init();

	// "KdGameObject"の派生クラスの名前をキーとしてインスタンスを生成する
	template <class ObjectType>
	void RegisterGameObjectFactoryMethod()
	{
		static_assert(std::is_base_of_v<KdGameObject, ObjectType> && "登録しようとしているクラスはKdGameObjectの派生クラスではありません");

		std::string className_ = typeid(ObjectType).name();

		StringUtility::StripClassPrefix(className_);
		
		if (m_gameObjectFactoryMethodList.contains(className_))
		{
			assert(false && "ゲームオブジェクトファクトリーメソッドの重複登録をしています、登録個所を確認してください");
			return;
		}

		auto lambda_ = []()
		{
			return std::make_shared<ObjectType>();
		};

		// 派生クラスが何を継承しているかを調べるためにインスタンス化を一回だけして調べる
		auto instance_ = lambda_();
		
		uint32_t baseID_ = instance_->GetFinalBaseTypeID();

		CommonStruct::GameObjectFactoryInfo factory_(lambda_, baseID_);

		// クラス名に応じたファクトリーメソッドを提供
		m_gameObjectFactoryMethodList.emplace(className_, factory_);

#ifdef _DEBUG
		std::string createLog_ = "\t[" + className_ + "] : successfully registered\n";
		KdDebugGUI::Instance().AddLog(createLog_.data());
#endif
	}

	const std::unordered_map<std::string, CommonStruct::GameObjectFactoryInfo>& GetGameObjectFactoryMethodList() const { return m_gameObjectFactoryMethodList; }

private:

	void RegisterGameObjectFactoryMethod();

	std::unordered_map<std::string, CommonStruct::GameObjectFactoryInfo> m_gameObjectFactoryMethodList;
};