#include "CameraBase.h"

#include "../../Utility/JsonUtility.h"
#include "../../Utility/CommonConstant.h"

#include "../Actor/Player/Player.h"

#include "../../Scene/BaseScene/BaseScene.h"
#include "../../Scene/SceneManager.h"

#include "../../../System/ImGui/ImGuiManager.h"
#include "../../../System/Input/RawInputManager.h"

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
	m_FixMousePos.x = CommonConstant::HALF_SCREEN_WIDTH;
	m_FixMousePos.y = CommonConstant::HALF_SCREEN_HEIGHT;

	m_isDebugMouseFree = false;
}

void CameraBase::PostLoadInit()
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	// もしシーンが存在しなければアクセスできないので"return"
	if (!currentScene_) { return; }

	// カメラのポインタをプレイヤーの"std::weak_ptr"に格納
	for (auto& cache_ : currentScene_->GetCacheObjectList<Player>())
	{
		m_targetPlayer = cache_;
	}
}

void CameraBase::Update()
{
	ToggleIsMouseFree();
}

void CameraBase::PostUpdate()
{
	FixMatrix();
}

void CameraBase::PreDraw()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(m_mWorld);
	m_spCamera->SetToShader();
}

void CameraBase::DrawImGuiInspectors()
{
	DrawImGuiTransformInspector();
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

void CameraBase::FixMatrix()
{
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	const std::shared_ptr<const KdGameObject>	_spTarget = GetTargetPlayer().lock();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	// カメラ座標の更新
	m_mLocalPos = Math::Matrix::CreateTranslation(m_location);

	m_mRotation = GetRotationMatrix();
	m_mWorld = m_mLocalPos * m_mRotation * _targetMat;
}

void CameraBase::DrawImGuiTransformInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Transform");

	ImGui::DragFloat3("Location", &m_location.x, 0.1f);
	ImGui::DragFloat3("Rotation", &m_degAng.x  , 1.0f);

	if (ImGui::Button("Reset Rotation"))
	{
		m_degAng = {};
	}
}

void CameraBase::ToggleIsMouseFree()
{
	// TODO
	auto& input_ = RawInputManager::GetInstance();

	if(input_.GetKeyState(VK_CONTROL))
	{
		if (input_.IsKeyPressedOnce('C'))
		{
			if (m_isDebugMouseFree)
			{
				m_isDebugMouseFree = false;
			}
			else
			{
				m_isDebugMouseFree = true;
			}
		}
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
	
	Math::Vector3 v = {};
}