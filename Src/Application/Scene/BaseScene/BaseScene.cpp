#include "BaseScene.h"

void BaseScene::PreUpdate()
{
	ClearExpiredGameObject         ();
	ClearExpiredGameObjectCacheList();

	for (auto& obj_ : m_objectList)
	{
		obj_->PreUpdate();
	}
}

void BaseScene::Update()
{
	// シーン毎のイベント処理
	Event();

	// KdGameObjectを継承した全てのオブジェクトの更新 (ポリモーフィズム)
	for (auto& obj_ : m_objectList)
	{
		obj_->Update();
	}
}

void BaseScene::PostUpdate()
{
	for (auto& obj_ : m_objectList)
	{
		obj_->PostUpdate();
	}
}

void BaseScene::PreDraw()
{
	for (auto& obj_ : m_objectList)
	{
		obj_->PreDraw();
	}
}

void BaseScene::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj_ : m_objectList)
		{
			obj_->GenerateDepthMapFromLight();
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(背景など)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& obj_ : m_objectList)
		{
			obj_->DrawUnLit();
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
		for (auto& obj_ : m_objectList)
		{
			obj_->DrawLit();
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(エフェクトなど)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& obj_ : m_objectList)
		{
			obj_->DrawEffect();
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& obj_ : m_objectList)
		{
			obj_->DrawBright();
		}
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void BaseScene::DrawSprite()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 2Dの描画はこの間で行う
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& obj_ : m_objectList)
		{
			obj_->DrawSprite();
		}
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void BaseScene::DrawDebug()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// デバッグ情報の描画はこの間で行う
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& obj_ : m_objectList)
		{
			obj_->DrawDebug();
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();
}

void BaseScene::Event()
{
	// 各シーンで必要な内容を実装(オーバーライド)する
}

void BaseScene::Init()
{
	// 各シーンで必要な内容を実装(オーバーライド)する
}

void BaseScene::ClearExpiredGameObject()
{
	// Updateの前の更新処理
	// オブジェクトリストの整理 ・・・ 無効なオブジェクトを削除
	auto itr_ = m_objectList.begin();

	while (itr_ != m_objectList.end())
	{
		if ((*itr_)->IsExpired())	// IsExpired() ・・・ 無効なら"true"
		{
			// 無効なオブジェクトをリストから削除
			itr_ = m_objectList.erase(itr_);
		}
		else
		{
			++itr_;	// 次の要素へイテレータを進める
		}
	}
	// ↑の後には有効なオブジェクトだけのリストになっている
}
void BaseScene::ClearExpiredGameObjectCacheList()
{
	// もしゲームオブジェクトが"Expired"で切れているなら"WeakPtr"からキャッシュ用の
	// "WeakPtr"を消す
	for (auto& [key_ , value_] : m_cachedObjectsByBaseType)
	{
		auto itr_ = value_.begin();
	
		while (itr_ != value_.end())
		{
			auto wp_ = itr_->lock();

			// もし有効な"std::weak_ptr<T>"でなければ消す
			if (wp_)
			{
				++itr_;
			}
			else
			{
				itr_ = value_.erase(itr_);
			}
		}
	}
}