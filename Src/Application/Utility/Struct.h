#pragma once

namespace Tag
{
	// 3Dのゲームオブジェクトに最低限必要なパラメータ
	struct Transform3D
	{
		Math::Vector3 scale    = Math::Vector3::Zero;
		Math::Vector3 rotation = Math::Vector3::Zero;
		Math::Vector3 location = Math::Vector3::Zero;
	};

	struct MaterialInfo
	{
		std::shared_ptr<KdModelWork> modelWork     = nullptr;
		std::string					 assetFilePath = "";
		Math::Color				     color         = {};
	};
}