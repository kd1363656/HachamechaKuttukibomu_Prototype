#include "CameraBase.h"

#include "../../Utility/InputManager/RawInputManager.h"
#include "../../Utility/JsonUtility.h"

void CameraBase::Init()
{
	KdGameObject::Init();

	m_degAng   = Math::Vector3::Zero;
	m_location = Math::Vector3::Zero;

	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}
	// ↓画面中央座標
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	m_isDebugMouseFree      = false;
	m_isPressDebugMouseFree = false;
}

void CameraBase::Update()
{
	ToggleIsMouseFree();
}

void CameraBase::PreDraw()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(m_mWorld);
	m_spCamera->SetToShader();
}

void CameraBase::ImGuiTransformInspector()
{
	ImGui::DragFloat3("Location", &m_location.x, 0.1f);
	ImGui::DragFloat3("Rotation", &m_degAng.x  , 1.0f);
}

void CameraBase::LoadJsonData(const nlohmann::json Json)
{
	m_typeName = Json.value("TypeName", "");

	m_degAng   = JsonUtility::JsonToVec3(Json["DegAng"  ]);
	m_location = JsonUtility::JsonToVec3(Json["Location"]);
}

nlohmann::json CameraBase::SaveJsonData()
{
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["DegAng"  ] = JsonUtility::Vec3ToJson(m_degAng  );
	json_["Location"] = JsonUtility::Vec3ToJson(m_location);

	return json_;
}

void CameraBase::SetTarget(const std::shared_ptr<KdGameObject>& target)
{
	if (!target) { return; }

	m_wpTarget = target;
}

void CameraBase::ToggleIsMouseFree()
{
	// TODO
	auto& input_ = RawInputManager::GetInstance();

	if(input_.GetKeyState(VK_CONTROL) && input_.GetKeyState('C'))
	{
		if (!m_isPressDebugMouseFree)
		{

			if (m_isDebugMouseFree)
			{
				m_isDebugMouseFree = false;
			}
			else
			{
				m_isDebugMouseFree = true;
			}

			m_isPressDebugMouseFree = true;
		}
	}
	else
	{
		m_isPressDebugMouseFree = false;
	}
}

void CameraBase::UpdateRotateByMouse()
{
	if (m_isDebugMouseFree) { return; }

	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	m_degAng.x += _mouseMove.y * 0.15f;
	m_degAng.y += _mouseMove.x * 0.15f;

	// 回転制御
	m_degAng.x = std::clamp(m_degAng.x, -45.f, 45.f);
}