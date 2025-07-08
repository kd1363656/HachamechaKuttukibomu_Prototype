#include "JsonUtility.h"

nlohmann::json JsonUtility::LoadJsonFile(const std::string& FilePath)
{
	std::filesystem::path path_(FilePath);
	if (path_.extension() != ".json")
	{
		path_ += ".json";
	}

	std::ifstream ifs_(path_);

	if (ifs_.fail()) { assert(false && "Jsonファイルが読み込めませんでした、ファイルパスを確認してください"); }

	nlohmann::json json_;
	ifs_ >> json_;
	ifs_.close();

	return json_;
}

void JsonUtility::SaveJsonFile(const nlohmann::json& Json, const std::string& FilePath)
{
	std::filesystem::path path_(FilePath);

	if (path_.extension() != ".json")
	{
		path_ += ".json";
	}

	std::ofstream ofs_(path_, std::ios::out);

	ofs_ << Json.dump(4);
	ofs_.close();
}

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

CommonStruct::RayDetail JsonUtility::JsonToRayDetail(const nlohmann::json& Json)
{
	CommonStruct::RayDetail rayDetail = {};

	if (Json.contains("Offset"   )) { rayDetail.offset    = JsonToVec3(Json["Offset"]   ); }
	if (Json.contains("Direction")) { rayDetail.direction = JsonToVec3(Json["Direction"]); }
	
	rayDetail.range         = Json.value("Range"         , 0.0f);
	rayDetail.collisionType = Json.value("CollisionType" , 0u  );

	return rayDetail;
}

nlohmann::json JsonUtility::RayDetailToJson(const CommonStruct::RayDetail& RayDetail)
{
	return nlohmann::json
	{
		{ "Offset"        , Vec3ToJson(RayDetail.offset   ) } ,
		{ "Direction"     , Vec3ToJson(RayDetail.direction) } ,
		{ "Range"         , RayDetail.range                 } ,
		{ "CollisionType" , RayDetail.collisionType         }
	};
}
