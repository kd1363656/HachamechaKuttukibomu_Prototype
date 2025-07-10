#pragma once

class BaseScene
{
public :

	BaseScene         () { Init(); }
	virtual ~BaseScene() {}

	void PreUpdate ();
	void Update    ();
	void PostUpdate();

	void PreDraw   ();
	void Draw      ();
	void DrawSprite();
	void DrawDebug ();

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjectList()
	{
		return m_objectList;
	}
	
	// オブジェクトリストに追加
	void AddObject(const std::shared_ptr<KdGameObject>& Obj)
	{
		m_objectList.push_back(Obj);

		AddObjectCache(Obj);
	}

	// 継承した基底クラスの"ID"と派生クラスの"ID"を登録
	void AddObjectCache(const std::shared_ptr<KdGameObject>& Obj)
	{
		const uint32_t baseID_    = Obj->GetFinalBaseTypeID();
		const uint32_t derivedID_ = Obj->GetTypeID         ();
		
		m_objectCacheList[baseID_].emplace_back   (Obj);
		m_objectCacheList[derivedID_].emplace_back(Obj);
	}

	template<class Type>
	std::list<std::weak_ptr<Type>> GetCacheObjectList()
	{
		static_assert(std::is_base_of_v<KdGameObject, Type> , "KdGameObjectが継承されていないクラスをリストに格納しようとしました、コードを確認してください");
		std::list<std::weak_ptr<Type>> result_;

		const uint32_t id_ = GameObjectID::GetTypeID<Type>();

		// IDと一致したキャッシュを格納したリストを安全にダウンキャスト
		if(m_objectCacheList.contains(id_))
		{
			for(auto& cache_ : m_objectCacheList[id_])
			{
				if(std::shared_ptr<KdGameObject> wp_ = cache_.lock())
				{
					std::weak_ptr<Type> casted_ = std::static_pointer_cast<Type>(wp_);

					result_.emplace_back(casted_);
				}
			}
		}

		return result_;
	}

protected :

	// 継承先シーンで必要ならオーバーライドする
	virtual void Event() { /* 各シーンで必要な内容を実装(オーバーライド)する */ }
	virtual void Init () { /* 各シーンで必要な内容を実装(オーバーライド)する */ }

	void ClearExpiredGameObjectCacheList();
	void ClearExpiredGameObject         ();

	// 全オブジェクトのアドレスをリストで管理
	std::list<std::shared_ptr<KdGameObject>> m_objectList;

	// 最終的な派生する前の中間基底クラスの"ID"をもとに構成されたキャッシュリスト
	std::unordered_map<uint32_t, std::list<std::weak_ptr<KdGameObject>>> m_objectCacheList;
};
