#pragma once

namespace CommonStruct
{
	struct GameObjectFactoryInfo
	{
		std::function<std::shared_ptr<KdGameObject>()> gameObjectFactoryMethod;
		const uint32_t								   finalBaseTypeID;

		GameObjectFactoryInfo(std::function<std::shared_ptr<KdGameObject>()> Factory , uint32_t FinalBaseTypeID) : 
			gameObjectFactoryMethod(Factory)        , 
			finalBaseTypeID        (FinalBaseTypeID)
		{}
	};

	// フラグはこのまま将来増えないので"bool"で管理
	struct KeyToggleInfo
	{
		int  keyCode   = NULL;
		bool isActive  = false;
		bool isKeyHeld = false;
	};

	// 3Dのゲームオブジェクトに最低限必要なパラメータ
	struct Transform3D
	{
		Math::Vector3 scale    = Math::Vector3::Zero;
		Math::Vector3 rotation = Math::Vector3::Zero;
		Math::Vector3 location = Math::Vector3::Zero;
	};

	struct MeshInfo
	{
		std::string	assetFilePath = "";
		Math::Color	color         = {};
	};

	// "ImGui"で使うだけのもの
	struct CollisionTypeList
	{
		const char* Label;
		uint32_t    Type;
	};
}