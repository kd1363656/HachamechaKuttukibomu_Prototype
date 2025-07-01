#pragma once
#include "../Utility/SingletonBase.h"

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
		
		if (m_gameObjectFactory.contains(className_))
		{
			assert(false && "ゲームオブジェクトファクトリーメソッドの重複登録をしています、登録個所を確認してください");
			return;
		}

		auto lambda_ = []()
		{
			return std::make_shared<ObjectType>();
		};

		// クラス名に応じたファクトリーメソッドを提供
		m_gameObjectFactory.emplace(className_ , lambda_);

#ifdef _DEBUG
		std::string createLog_ = "\t" + className_ + " : successfully registered\n";
		KdDebugGUI::Instance().AddLog(createLog_.data());
#endif
	}

private:

	void RegisterGameObjectFactoryMethod();

	std::unordered_map<std::string, std::function<std::shared_ptr<KdGameObject>()>> m_gameObjectFactory;

};