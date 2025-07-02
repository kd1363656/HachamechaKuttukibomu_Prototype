#pragma once
#include "../Utility/SingletonBase.h"
#include "../Utility/Struct.h"

// "class"は必ず"class Name"と書く、さもなければバグが発生する
class Factory : public SingletonBase<Factory>
{
public:

	Factory ()         = default;
	~Factory()override = default;

	void Init();

	void StripClassPrefix(std::string& ClassName);

	// "KdGameObject"の派生クラスの名前をキーとしてインスタンスを生成する
	template <class ObjectType>
	void RegisterGameObjectFuctoryMethod()
	{
		static_assert(std::is_base_of_v<KdGameObject, ObjectType> && "登録しようとしているクラスはKdGameObjectの派生クラスではありません");

		std::string className_ = typeid(ObjectType).name();

		StripClassPrefix(className_);
		
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

		Tag::GameObjectFactoryInfo factory_(lambda_, baseID_);

		// クラス名に応じたファクトリーメソッドを提供
		m_gameObjectFactoryMethodList.emplace(className_, factory_);

#ifdef _DEBUG
		std::string createLog_ = "\t[" + className_ + "] : successfully registered\n";
		KdDebugGUI::Instance().AddLog(createLog_.data());
#endif
	}

	const std::unordered_map<std::string, Tag::GameObjectFactoryInfo>& GetGameObjectFactoryMethodList() const { return m_gameObjectFactoryMethodList; }

private:

	void RegisterGameObjectFactoryMethod();

	std::unordered_map<std::string, Tag::GameObjectFactoryInfo> m_gameObjectFactoryMethodList;
};