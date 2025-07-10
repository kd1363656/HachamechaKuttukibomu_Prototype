#pragma once
#include "CommonStruct.h"

namespace JsonUtility
{
	inline Math::Vector2  JsonToVec2(const nlohmann::json& Json) { return Math::Vector2  { Json.value("X"   , 0.0f) , Json.value("Y" , 0.0f) };   }
	inline nlohmann::json Vec2ToJson(const Math::Vector2&  Vec2) { return nlohmann::json { { "X" , Vec2.x } ,                   {"Y" , Vec2.y} }; }

	inline Math::Vector3  JsonToVec3(const nlohmann::json& Json ) { return Math::Vector3 { Json.value("X" , 0.0f) , Json.value("Y" , 0.0f) , Json.value("Z" , 0.0f) }; }
	inline nlohmann::json Vec3ToJson(const Math::Vector3&  Value) { return nlohmann::json{ {"X" , Value.x}        , {"Y" , Value.y}        , {"Z" , Value.z}	    }; }

	inline Math::Vector4  JsonToVec4(const nlohmann::json& Json) { return Math::Vector4 { Json.value("X" , 0.0f) , Json.value("Y" , 0.0f) , Json.value("Z" , 0.0f) , Json.value("W" , 0.0f) }; }
	inline nlohmann::json Vec4ToJson(const Math::Vector4&  Vec4) { return nlohmann::json{ {"X" , Vec4.x }        , {"Y" , Vec4.y }        , {"Z" , Vec4.z }        , {"W" , Vec4.w}         }; }

	inline Math::Color    JsonToColor(const nlohmann::json& Json ) { return Math::Color    { Json.value("X"   , 0.0f) , Json.value("Y" , 0.0f) , Json.value("Z"  , 0.0f) , Json.value("W" , 0.0f) }; }
	inline nlohmann::json ColorToJson(const Math::Color&    Color) { return nlohmann::json { { "X" , Color.x }        , {"Y" , Color.y}        , {"Z" , Color.z} , {"W" , Color.w  }              }; }

	// 自作構造体用ヘルパー関数
	inline CommonStruct::GravityInfo JsonToGravityInfo(const nlohmann::json&            Json       ) { return CommonStruct::GravityInfo{ Json.value("CurrentGravity", 0.0f)               , Json.value("AccumulatedGravity" , 0.0f)      , Json.value("MaxGravity" , 0.0f)          }; }
	inline nlohmann::json            GravityInfoToJson(const CommonStruct::GravityInfo& GravityInfo) { return nlohmann::json{ { "CurrentGravity", GravityInfo.currentGravity } , {"AccumulatedGravity" , GravityInfo.accumulatedGravity} , {"MaxGravity" , GravityInfo.maxGravity } }; }

	CommonStruct::Transform3D JsonToTransform3D(const nlohmann::json&           Json     );
	nlohmann::json            Transform3DToJson(const CommonStruct::Transform3D Transform);

	CommonStruct::MeshInfo JsonToMeshInfo(const nlohmann::json&         Json    );
	nlohmann::json         MeshInfoToJson(const CommonStruct::MeshInfo& MeshInfo);

	CommonStruct::RayColliderSetting JsonToRayColliderSetting(const nlohmann::json&                   Json              );
	nlohmann::json                   RayColliderSettingToJson(const CommonStruct::RayColliderSetting& RayColliderSetting);

	CommonStruct::SphereColliderSetting JsonToSphereColliderSetting(const nlohmann::json&                      Json                 );
	nlohmann::json                      SphereColliderSettingToJson(const CommonStruct::SphereColliderSetting& SphereColliderSetting);
}