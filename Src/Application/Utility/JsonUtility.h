#pragma once

namespace JsonUtility
{
	nlohmann::json LoadJsonFile(const std::string& FilePath);
	void SaveJsonFile(const nlohmann::json& Json, const std::string& FilePath);

	inline Math::Vector2  JsonToVec2(const nlohmann::json& Json) { return Math::Vector2  { Json.value("X"   , 0.0f) , Json.value("Y" , 0.0f) };   }
	inline nlohmann::json Vec2ToJson(const Math::Vector2&  Vec2) { return nlohmann::json { { "X" , Vec2.x } ,                   {"Y" , Vec2.y} }; }

	inline Math::Vector3  JsonToVec3(const nlohmann::json& Json ) { return Math::Vector3 { Json.value("X" , 0.0f) , Json.value("Y" , 0.0f) , Json.value("Z" , 0.0f) }; }
	inline nlohmann::json Vec3ToJson(const Math::Vector3&  Value) { return nlohmann::json{ {"X" , Value.x}        , {"Y" , Value.y}        , {"Z" , Value.z}	    }; }

	inline Math::Vector4  JsonToVec4(const nlohmann::json& Json) { return Math::Vector4{ Json.value("X" , 0.0f) , Json.value("Y" , 0.0f) , Json.value("Z" , 0.0f) , Json.value("W" , 0.0f) }; }
	inline nlohmann::json Vec4ToJson(const Math::Vector4&  Vec4) { return nlohmann::json{ {"X" , Vec4.x }       , {"Y" , Vec4.y }        , {"Z" , Vec4.z }        , {"W" , Vec4.w}         }; }

	inline Math::Color    JsonToColor(const nlohmann::json& Json ) { return Math::Color    { Json.value("X"   , 0.0f) , Json.value("Y" , 0.0f) , Json.value("Z"  , 0.0f) , Json.value("W" , 0.0f) }; }
	inline nlohmann::json ColorToJson(const Math::Color&    Color) { return nlohmann::json { { "X" , Color.x }        , {"Y" , Color.y}        , {"Z" , Color.z} , {"W" , Color.w  }              }; }
}