#include "ActorBase.h"

#include "../../Factory/Factory.h"

void ActorBase::Init()
{
	KdGameObject::Init();
	
	m_transform    = {};
	m_materialInfo = {};
}

void ActorBase::DrawLit()
{
	if (!m_materialInfo.modelWork)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_materialInfo.modelWork , m_mWorld);
}

void ActorBase::Update()
{
	FixMatrix();
}

void ActorBase::ImGuiInspector()
{
	ImGui::DragFloat3("Location" , &m_transform.location.x , 0.1f);
	ImGui::DragFloat3("Rotation" , &m_transform.rotation.x , 1.0f);
	ImGui::DragFloat3("Scale"    , &m_transform.scale.x    , 0.1f);
}

void ActorBase::FixMatrix()
{
	const Math::Matrix transMat_    = Math::Matrix::CreateTranslation     (m_transform.location);
	const Math::Matrix rotationMat_ = Math::Matrix::CreateFromYawPitchRoll(m_transform.rotation);
	const Math::Matrix scaleMat_    = Math::Matrix::CreateScale           (m_transform.scale   );

	m_mWorld = transMat_ * rotationMat_ * scaleMat_;
}