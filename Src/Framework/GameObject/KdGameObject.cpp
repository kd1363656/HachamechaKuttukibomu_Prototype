﻿#include "KdGameObject.h"

#include "../Src/Application/Utility/StringUtility.h"

void KdGameObject::Init()
{
	m_mWorld = Math::Matrix::Identity;

	// "Json"ファイルに登録するためだけに使用
	// 本来なら"typeid"はコンパイラ依存なので使用すべきでない(実装優先なので細かいことはしない)
	m_typeName = typeid(*this).name();
	// 名前を整形
	StringUtility::StripClassPrefix(m_typeName);

	m_isExpired = false;

	if (!m_pDebugWire) { m_pDebugWire = std::make_unique<KdDebugWireFrame>(); }
}

void KdGameObject::DrawDebug()
{
	// 早期リターン
	if (!m_pDebugWire)return;

	m_pDebugWire->Draw();
}

void KdGameObject::CalcDistSqrFromCamera(const Math::Vector3& camPos)
{
	m_distSqrFromCamera = (m_mWorld.Translation() - camPos).LengthSquared();
}

bool KdGameObject::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults) const
{
	if (!m_pCollider) { return false; }

	return m_pCollider->Intersects(targetShape, m_mWorld, pResults);
}
bool KdGameObject::Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults) const
{
	if (!m_pCollider) { return false; }

	return m_pCollider->Intersects(targetBox, m_mWorld, pResults);
}
bool KdGameObject::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults) const
{
	if (!m_pCollider) { return false; }

	return m_pCollider->Intersects(targetShape, m_mWorld, pResults);
}

Math::Vector3 KdGameObject::GetScale() const
{
	return Math::Vector3(m_mWorld.Right().Length(), m_mWorld.Up().Length(), m_mWorld.Backward().Length());
}

void KdGameObject::SetScale(float scalar)
{
	Math::Vector3 scale(scalar);

	SetScale(scale);
}

void KdGameObject::SetScale(const Math::Vector3& scale)
{
	Math::Vector3 vecX = m_mWorld.Right   ();
	Math::Vector3 vecY = m_mWorld.Up      ();
	Math::Vector3 vecZ = m_mWorld.Backward(); 
	
	vecX.Normalize();
	vecY.Normalize();
	vecZ.Normalize();

	m_mWorld.Right   (vecX * scale.x);
	m_mWorld.Up      (vecY * scale.y);
	m_mWorld.Backward(vecZ * scale.z);
}