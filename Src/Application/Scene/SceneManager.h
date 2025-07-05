#pragma once
#include "../Utility/SingletonBase.h"

#include "../FileIO/GameObject/GameObjectFileIO.h"

class BaseScene;

class SceneManager : public SingletonBase<SceneManager>
{
public :

	SceneManager () {}
	~SceneManager() {}

	// シーン情報
	enum class SceneType
	{
		Title ,
		Game  ,
	};

	// マネージャーの初期化
	// インスタンス生成(アプリ起動)時にコンストラクタで自動実行
	void Init();

	void PreUpdate ();
	void Update    ();
	void PostUpdate();

	void PreDraw   ();
	void Draw      ();
	void DrawSprite();
	void DrawDebug ();

	// 現在のシーンにオブジェクトを追加
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

	// 次のシーンをセット (次のフレームから切り替わる)
	void SetNextScene(SceneType _nextScene)
	{
		m_nextSceneType = _nextScene;
	}

	// 現在のシーンのオブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	std::weak_ptr<BaseScene> GetCurrentScene() { return m_currentScene; }

	const std::unique_ptr<GameObjectFileIO>& GetGameObjectFileIO()const { return m_gameObjectFileIO; }

private :

	// シーン切り替え関数
	void ChangeScene(SceneType _sceneType);

	// 現在のシーンのインスタンスを保持しているポインタ
	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	// 現在のシーンの種類を保持している変数
	SceneType m_currentSceneType = SceneType::Game;
	
	// 次のシーンの種類を保持している変数
	SceneType m_nextSceneType = m_currentSceneType;

	std::unique_ptr<GameObjectFileIO> m_gameObjectFileIO = nullptr;
};
