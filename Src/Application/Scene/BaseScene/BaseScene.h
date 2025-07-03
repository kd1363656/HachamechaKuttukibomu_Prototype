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

		AddObjectByBaseTypeID(Obj);
	}

	// 最終的に継承された基底クラスの"ID"でキャッシュを識別して格納
	void AddObjectByBaseTypeID(const std::shared_ptr<KdGameObject>& Obj)
	{
		const uint32_t id_ = Obj->GetFinalBaseTypeID();

		m_cachedObjectsByBaseType[id_].emplace_back(Obj);
	}

	template<class BaseType>
	std::list<std::weak_ptr<BaseType>> GetCacheObjectList()
	{
		static_assert(std::is_base_of_v<KdGameObject, BaseType> , "KdGameObjectが継承されていないクラスをリストに格納しようとしました、コードを確認してください");
		std::list<std::weak_ptr<BaseType>> result_;

		const uint32_t id_ = GameObjectID::GetTypeID<BaseType>();

		// IDと一致したキャッシュを格納したリストを安全にダウンキャスト
		if(m_cachedObjectsByBaseType.contains(id_))
		{
			for(auto& cache_ : m_cachedObjectsByBaseType[id_])
			{
				if(std::shared_ptr<KdGameObject> wp_ = cache_.lock())
				{
					std::weak_ptr<BaseType> casted_ = std::static_pointer_cast<BaseType>(wp_);

					result_.emplace_back(casted_);
				}
			}
		}

		return result_;
	}

protected :

	// 継承先シーンで必要ならオーバーライドする
	virtual void Event();
	virtual void Init ();

	void ClearExpiredGameObjectCacheList();
	void ClearExpiredGameObject         ();

	// 全オブジェクトのアドレスをリストで管理
	std::list<std::shared_ptr<KdGameObject>> m_objectList;

	// 最終的な派生する前の中間基底クラスの"ID"をもとに構成されたキャッシュリスト
	std::unordered_map<uint32_t, std::list<std::weak_ptr<KdGameObject>>> m_cachedObjectsByBaseType;
};
