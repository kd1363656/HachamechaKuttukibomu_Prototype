#include "JsonUtility.h"

CommonStruct::Transform3D JsonUtility::JsonToTransform3D(const nlohmann::json& Json)
{
	return CommonStruct::Transform3D
	{ 
		Json.contains("Scale")    ? JsonToVec3(Json["Scale"   ]) : Math::Vector3::Zero , 
		Json.contains("Rotation") ? JsonToVec3(Json["Rotation"]) : Math::Vector3::Zero , 
		Json.contains("Location") ? JsonToVec3(Json["Location"]) : Math::Vector3::Zero
	};
}

nlohmann::json JsonUtility::Transform3DToJson(const CommonStruct::Transform3D Transform)
{
	return nlohmann::json
	{
		{ "Scale"    , Vec3ToJson(Transform.scale)    } ,
		{ "Rotation" , Vec3ToJson(Transform.rotation) } ,
		{ "Location" , Vec3ToJson(Transform.location) }
	};
}

CommonStruct::MeshInfo JsonUtility::JsonToMeshInfo(const nlohmann::json& Json)
{
	return CommonStruct::MeshInfo
	{
		Json.value   ("AssetFilePath" , "")									,
		Json.contains("Color") ? JsonToColor(Json["Color"]) : Math::Color{}
	};
}
nlohmann::json JsonUtility::MeshInfoToJson(const CommonStruct::MeshInfo& MeshInfo)
{
	return nlohmann::json
	{
		{ "AssetFilePath" , MeshInfo.assetFilePath      } ,
		{ "Color"         , ColorToJson(MeshInfo.color) }
	};
}

CommonStruct::RayColliderSetting JsonUtility::JsonToRayColliderSetting(const nlohmann::json& Json)
{
	CommonStruct::RayColliderSetting rayDetail = {};

	if (Json.contains("Offset"   )) { rayDetail.offset    = JsonToVec3(Json["Offset"]   ); }
	if (Json.contains("Direction")) { rayDetail.direction = JsonToVec3(Json["Direction"]); }
	
	rayDetail.range         = Json.value("Range"         , 0.0f);
	rayDetail.collisionType = Json.value("CollisionType" , 0u  );

	return rayDetail;
}

nlohmann::json JsonUtility::RayColliderSettingToJson(const CommonStruct::RayColliderSetting& RayColliderSetting)
{
	return nlohmann::json
	{
		{ "Offset"        , Vec3ToJson(RayColliderSetting.offset   ) } ,
		{ "Direction"     , Vec3ToJson(RayColliderSetting.direction) } ,
		{ "Range"         , RayColliderSetting.range                 } ,
		{ "CollisionType" , RayColliderSetting.collisionType         }
	};
}

CommonStruct::SphereColliderSetting JsonUtility::JsonToSphereColliderSetting(const nlohmann::json& Json)
{
	CommonStruct::SphereColliderSetting sphereColliderSetting_ = {};

	if (Json.contains("Offset")) { sphereColliderSetting_.offset = JsonToVec3(Json["Offset"]); }

	sphereColliderSetting_.radius        = Json.value("Radius"        , 0.0f);
	sphereColliderSetting_.collisionType = Json.value("CollisionType" , 0u  );

	return sphereColliderSetting_;
}

nlohmann::json JsonUtility::SphereColliderSettingToJson(const CommonStruct::SphereColliderSetting& SphereColliderSetting)
{
	return nlohmann::json
	{
		{ "Offset"        , Vec3ToJson(SphereColliderSetting.offset) } , 
		{ "Radius"        , SphereColliderSetting.radius             } ,
		{ "CollisionType" , SphereColliderSetting.collisionType      }
	};
}