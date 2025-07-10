#pragma once

// ゲーム上に存在するすべてのオブジェクトの基底となるクラス
// 派生クラスの"Init"でフィルター分けをするために"TypeID"をセットしているが"KdGameObject"ではしない
// "KdGameObject"から派生したクラスでフィルター分けしないと意味がないから
class KdGameObject : public std::enable_shared_from_this<KdGameObject>
{
public:

	// どのような描画を行うのかを設定するTypeID：Bitフラグで複数指定可能
	enum class DrawType
	{
		Lit                       = 1 << 0 ,
		UnLit                     = 1 << 1 ,
		Effect                    = 1 << 2 ,
		Bright                    = 1 << 3 ,
		UI                        = 1 << 4 ,
		Sprite                    = 1 << 5 ,
		GenerateDepthFromMapLight = 1 << 6 ,
	};

	struct DrawTypeList
	{
		const char* label;
		uint8_t     type;
	};

	// "ImGui"で使うだけのもの
	struct CollisionTypeList
	{
		const char* label;
		uint8_t     type;
	};

	KdGameObject() = default;
	virtual ~KdGameObject() { Release(); }

	// 安全にダウンキャストするために必要な"ID"とても重要
	virtual uint32_t GetTypeID() const = 0;
	// 派生する前の最後の基底クラスのみを知りたいときに使う
	virtual uint32_t GetFinalBaseTypeID() const = 0;

	virtual void Init        ();
	virtual void PostLoadInit();

	virtual void PreUpdate () { /* まだ実装されていません、派生クラスで実装してください */ }
	virtual void Update    () { /* まだ実装されていません、派生クラスで実装してください */ }
	virtual void PostUpdate() { /* まだ実装されていません、派生クラスで実装してください */ }

	// それぞれの状況で描画する関数
	virtual void PreDraw                  () { /* まだ実装されていません、派生クラスで実装してください */ }
	virtual void Draw                     () { /* まだ実装されていません、派生クラスで実装してください */ }
	virtual void DrawDebug				  ();

	// "ImGuiManager"で管理するよりこちらで管理したほうが柔軟性、拡張性が高いからここに書く
	virtual void DrawImGuiInspectors        ();
	void         DrawImGuiDrawTypeInspector ();
	void         DrawImGuiCollisionInspector();

	virtual void           LoadJsonData(const nlohmann::json Json) {/* まだ実装されていません、派生クラスで実装してください */ }
	virtual nlohmann::json SaveJsonData()						   { return nlohmann::json(); }

	// カメラからの距離を計算
	virtual void CalcDistSqrFromCamera(const Math::Vector3& camPos);
	// 視錐台範囲内に入っているかどうか
	virtual bool CheckInScreen(const DirectX::BoundingFrustum&) const { return false; }

	virtual bool IsExpired() const { return m_isExpired; }

	virtual bool IsVisible () const { return false; }
	virtual bool IsRideable() const { return false; }

	bool Intersects(const KdCollider::SphereInfo& targetShape , std::list<KdCollider::CollisionResult>* pResults) const;
	bool Intersects(const KdCollider::BoxInfo&    targetBox   , std::list<KdCollider::CollisionResult>* pResults) const;
	bool Intersects(const KdCollider::RayInfo&    targetShape , std::list<KdCollider::CollisionResult>* pResults) const;

	bool HasDrawTypeFlag(DrawType Type);

	void EnableDrawFlag (DrawType Type);
	void DisableDrawFlag(DrawType Type);

	virtual Math::Vector3 GetPos  () const { return m_mWorld.Translation(); }
	virtual Math::Vector3 GetScale() const;

	const Math::Matrix& GetMatrix() const { return m_mWorld; }

	std::string_view GetTypeName()const { return m_typeName; }

	float GetDistSqrFromCamera() const { return m_distSqrFromCamera; }

	uint8_t GetDrawType() const { return m_drawType; }

	// 呼び出した行のローカル変数がなくなっても"std::string"ではコピーが発生するので問題ない
	void SetTypeName(const std::string_view Set) { m_typeName = Set; }

	virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	
	// 拡大率を変更する関数
	void         SetScale(float scalar);
	virtual void SetScale(const Math::Vector3& scale);
	
	void SetIsExpired(bool Set) { m_isExpired = Set; }

protected:

	virtual void LoadAsset() {/* まだ実装されていません、派生クラスで実装してください */ }

	void Release() { /* まだ実装されていません、派生クラスで実装してください */ }

	const std::string COMMON_ASSET_FILE_PATH = "Asset/";

	std::unique_ptr<KdCollider> m_pCollider = nullptr;

	std::unique_ptr<KdDebugWireFrame> m_pDebugWire = nullptr;

	Math::Matrix m_mWorld = Math::Matrix::Identity;

	// クラス名(ゲッター関数の名前が被ってしまうから)
	std::string m_typeName = "";

	float m_distSqrFromCamera = 0;

	uint8_t m_drawType      = 0u;
	uint8_t m_collisionType = 0u;

	bool m_isExpired = false;
};

// ゲームオブジェクトを識別するためのIDを割り振るクラス
// シングルトンにしたくもなくインスタンス化してほしくもない
// 必要に応じて"ID"を取得するためだけのクラス
// マルチスレッドにも対応(するかもしれないから)
class GameObjectID
{
public:

	template <class ObjectType>
	static inline uint32_t GetTypeID()
	{
		static_assert(std::is_base_of_v<KdGameObject, ObjectType>, "KdGameObjectが継承されていないクラスにIDを付与しようとしています、コードを確認してください");
		static uint32_t id_ = GenerateID();
		return id_;
	}

private:

	static inline uint32_t GenerateID()
	{
		static std::atomic<uint32_t> id_{ 0u };
		return id_++;
	}

	GameObjectID () = default;
	~GameObjectID() = default;
};